#include "msp430x16x.h"	// Systemdefinitionen von TI für den MSP430F1612
#include "../init.h"	// Initialisierung des Mikrocontrollers
#include "../CC1100.h"	// CC1100 Funktransceiver
#include "../system.h"	// Systemfunktionen MSB430H
#include "interrupts.h"	// ISR - Interrupt Service Routinen
#include "stdio.h"		// includes TI MSP430F1612 
#include "../SHT11.h"	// SHT11 Temperatur- und Feuchtesensor
#include "project.h"
#include "math.h"

// Arrays welche die Kordinaten der auszugebenden Punkte beinhalten
int valuesX[ANTZ];
int valuesY[ANTZ];

int i;

volatile int ic=2;

// Structs welche die Schlaeger beschreiben
barstate left_bar;
barstate right_bar;

int life_left;	// Player 1 auf cahnnel 2
int life_right; // Player 2 auf channel 3
int running;

// Struct welches den Zustand des Balls beschreibt
ballstate ball;

// Anzahl der Spieler
int player;
// Gibt an ob genug spieler Bereit sind fuer ein Spiel
int start_game;

char str[255];

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoegern
void delay(unsigned int time_mill) {
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

// Defines um einige Berechungen lesbarer zu machen
#define MAX(s1,s2) ((s1 > s2) ? s1 : s2) 
#define MIN(s1,s2) ((s1 < s2) ? s1 : s2)
#define GRAD(g) (((g) / 360.0) * 2.0 * M_PI)

void project(){
	int count;
	
	// DMA vorbereiten
	DMACTL0 = DMA0TSEL_8 + DMA1TSEL_8;
	DMACTL1 = ROUNDROBIN;

	DMA0CTL = DMASRCINCR_3 + DMADT_4 + DMAEN;
	DMA0SZ = ANTZ;
	DMA0SA = (unsigned int) valuesX;
	DMA0DA = (unsigned int) &DAC12_0DAT;

	DMA1CTL = DMASRCINCR_3 + DMADT_4 + DMAEN;
	DMA1SZ = ANTZ;
	DMA1SA = (unsigned int) valuesY;
	DMA1DA = (unsigned int) &DAC12_1DAT;

	// DAC vorbereiten
	DAC12_1CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1 + DAC12ENC + DAC12LSEL0;
	DAC12_0CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1 + DAC12ENC + DAC12LSEL0;

	//Timer und Output einstellen
	TBCTL = MC_1 + TASSEL_2 + ID_3;
	TBCCR0 = 100;

	// Interupts fuer Taster aktivieren
	P1IE |= 0x03;
	P1IES &= ~0x03;
    
	// Beschleunigungssensor als Randomquelle
	P6SEL = 0x01;
	ADC12MCTL0 = SREF_0 + INCH_0 + EOS;
	ADC12CTL1 = CONSEQ_1 + SHP;
	ADC12CTL0 = ADC12ON + ENC + ADC12SC + MSC;
	//GS1 und GS2 (empfindlichkeit) einstellen
	P5DIR |= (64+32+16);
	P5OUT &= ~(32 + 16);
	P5OUT |= 64; // Wake up

	ADC12IE = 0x01; // AD-Wandler Interupt einschalten
	_bis_SR_register(GIE); //Interrupts zulassen

	// ENC Toggle um eine Wandlung zu starten
	ADC12CTL0 |=  ADC12SC;
	ADC12CTL0 &= ~ADC12SC;    

	// Werte Initialisieren
	DAC12_1DAT=valuesY[0];
	DAC12_0DAT=valuesX[0];
	player = 0;
	start_game = 0;

	// Funk vorbereiten
	initUART0_SPI();
	initCC1100();
	setUid(0);
	switchFreq(7);

	// here be pong
	init(&ball);

	while(1) {
		int dir;

		if (running) {
			// naechsten Punkt fuer den Ball berechnen
			if((dir = next(&ball, left_bar, right_bar)) != IN) {
				// Wenn der Ball ausserhalb des spielfelds ist und
				// ein spieler keine Leben mehr hat, spiel anhalten
				if ((life_left < 0 || life_right < 0) && player == 2) {
					running = 0;
					LED_OFF(GREEN);
					LED_ON(RED);
					// TODO WIN und FAIL bestimmen
				}
			// Andernfalls den Spielern die Anzahl ihrer Leben
			// zuschicken und den Ball reseten
			else {
				char buf[2] = {'L','0'};
					reset_ball(&ball, dir);
					
					// verbleibende Leben senden
					buf[1] = '0' + life_left;
					sendPacket(2, 0, buf, 2);
					buf[1] = '0' + life_right;
					sendPacket(3, 0, buf, 2);
				}
			}

			// Wenn noch nicht genug Spieler da sind, zeige 2 "KIs" die
			// perfekt Pong spielen
			if (player < 2) {
				left_bar = MAX(0 + BARLENGTH/2.0, MIN(FIELD_Y - BARLENGTH/2.0, ball.y));
				right_bar = MAX(0 + BARLENGTH/2.0, MIN(FIELD_Y - BARLENGTH/2.0, ball.y));
			}
			
			// Erzeuge das Array mit den zu malenden Bildpunkten neu
			generate_array(&ball, left_bar, right_bar);

		}

		// Auf los gehts los
		if (start_game) {
			running = 0;
			start_game_now();
		}

		// Wenn noch nicht genug spieler da sind alle 100 druchlaeufe
		// ein Wainting for players Packet senden
		if (player < 2) {
			// jedes hundertste Mal
			if ((++count % 100) == 0) {
				count = 0;

				// W = WAITING_FOR_PLAYERS
				sendPacket(1, 0, "W", 1);
				LED_TOGGLE(YELLOW);
			}
		}
	}
}

// Diese Funktion berechnet die naechste Position des Balls
int next(ballstate *ball, barstate bar_left, barstate bar_right)
{
        float m_x, m_y;
        
        // Richtungsvektoren der Bewegung berechnen
        m_x = ball->speed * cos(ball->angle);
        m_y = ball->speed * sin(ball->angle);
        
        // Vektoren zur aktuellen Position addieren
        ball->x += m_x;
        ball->y += m_y;
        
        // Kollisionskontrolle
        // Ball hat einen Rand des Spielfeld erreicht
        // Refelktion berechnen
        if ((ball->y <= 0) || (ball->y >= FIELD_Y)) {
            ball->angle = normalize_radiant(2*M_PI - ball->angle);
            
            if (ball->y >= FIELD_Y) {
                // Ball trifft Wand bei yMax
                ball->y = 2 * FIELD_Y - ball->y;
            }
            else {
                // Ball trifft Wand bei yMin
                ball->y = fabs(ball->y);
            }
            // Geschwindikeit des Balls erhoehen (mehr Spass)
            ball->speed += 0.5;
        }
        
        // Ball ist auf einer Spielerseite angekommen
        if(ball->x <= 0 || ball->x >= FIELD_X){

			// Variable zur bestimmung Wo der Schlaeger getroffen wurde
			// um den Reflektionswinkel davon abhaenging zu machen
            float bar_position;
            
            if (ball->x <= 0) {
	            float collision_point_y = fabs(tan(ball->angle) * (ball->x - m_x) + (ball->y - m_y));
                bar_position = collision_point_y - bar_left;

                // Ball trifft Ebene bei xMin
                ball->x = fabs(ball->x);
            }
            else {
	            float collision_point_y = fabs(tan(ball->angle) * (FIELD_X - ball->x - m_x) + (ball->y - m_y));
                bar_position = bar_right - collision_point_y;

                // Ball trifft Ebene bei xMax
                ball->x = 2 * FIELD_X - ball->x;
            }

            if (fabs(bar_position) <= (BARLENGTH/2.0)) {
                ball->angle = normalize_radiant((M_PI - ball->angle) + (bar_position / (BARLENGTH/2.0)) * GRAD(30.0));

				// Dafuer sorgen das der Reflektionswinkel nicht zu
				// steil wird
				if (ball->angle > GRAD(70.0) && ball->angle < GRAD(110.0)) {
					if (ball->angle > GRAD(90.0)) {
						ball->angle = GRAD(110.0);
					}
					else {
						ball->angle = GRAD(70.0);
					}
				}
				else if (ball->angle > GRAD(250.0) && ball->angle < GRAD(290.0)) {
					if (ball->angle > GRAD(270.0)) {
						ball->angle = GRAD(290.0);
					}
					else {
						ball->angle = GRAD(250.0);
					}
				}

                // Geschwindigkeit des Ball erhoehen und getroffen
                // zurueckgeben wenn der Ball getroffen wurde
                ball->speed += 1;
                return IN;
            }
            
            // Entscheiden welche Seite Schuld war im Falle, dass der
            // Ball nicht getroffen wird
            if (ball->x > (FIELD_X / 2.0)) {
            	life_left--;
            	return LEFT;
            }

            life_right--;
            return RIGHT;
        }

        return IN;
}

// Funktion erstellt die Arrays fuer die Ausgabe auf dem Oszi
void generate_array(ballstate *ball, barstate bar_left, barstate bar_right)
{
    int i, count;
    // Punkte fuer den Ball erstellen
    for(i=0;i<BALL_POINTS;i++){
        valuesX[i] = (int)(ball->x * RES_X/FIELD_X);
        valuesY[i] = (int)(ball->y * RES_Y/FIELD_Y);
    }
    
    // Buchfuehrung
    count = ((ANTZ-BALL_POINTS) / 2);
    
    // Linken schlaeger malen
    for (i = 1; i < count + 1; i++) {
        valuesX[i + BALL_POINTS-1] = 0;
        valuesY[i + BALL_POINTS-1] = (int)(((bar_left - BARLENGTH/2.0) + (BARLENGTH / count) * i) * RES_Y/FIELD_Y);
    }
    
    //Rechten schlaeger malen
    for (i = count + 1; i <= ANTZ - BALL_POINTS; i++) {
        valuesX[i + BALL_POINTS-1] = (int)RES_X;
        valuesY[i + BALL_POINTS-1] = (int)(((bar_right - BARLENGTH/2.0) + (BARLENGTH / count) * (i-count)) * RES_Y/FIELD_Y);
    }
}

// Funktion die alle werte fuer ein Spiel intialisiert
void init(ballstate *ball)
{
	left_bar = FIELD_Y / 2;
	right_bar = FIELD_Y / 2;
	life_left = 1;
	life_right = 1;
	
	if (irand(2) == 0) {
		reset_ball(ball, -1);
	}
	else {
		reset_ball(ball, 1);
	}

	running = 1;
}

// Funktion die den Ball zuruecksetzt und mit zufaelligenm Wikel starten
// laesst
void reset_ball(ballstate *ball, int direction)
{
    ball->x = FIELD_X / 2 + (FIELD_X / 4.0 * -1.0 * direction);
    ball->y = FIELD_Y / 2;
    ball->speed = 5;
    
    if (direction < 0) {
   		ball->angle = GRAD(135.0 + (float)irand(90));
    }
    else {
   		ball->angle = normalize_radiant(GRAD(315.0 + (float)irand(90)));
    }
}

// Funktion welche einen Rad annimmt und zwischen 0 und 2*Pi
// normalisiert
float normalize_radiant(float source)
{
	float new;
	int ganzzahl;
	
	ganzzahl = (int)(source / (2.0 * M_PI));
	
	new = source - ganzzahl * (2.0 * M_PI);
	
	if (new < 0) {
		new = 2.0 * M_PI + new;
	}
	
	return new;
}

//Funktion startet ein neues Spiel
void start_game_now()
{
	char buf[2] = {'L','0'};
	init(&ball);
	generate_array(&ball, left_bar, right_bar);
	
	// neues spiel neue leben
	life_left=3;
	life_right=3;
	
	// Ampelssequenz
	delay(1000);
	
	LED_ON(RED);
	LED_ON(YELLOW);
	LED_ON(GREEN);
	
	delay(1000);
	
	LED_OFF(GREEN);

	delay(1000);

	LED_OFF(YELLOW);
	
	delay(1000);
	
	LED_OFF(RED);
	LED_ON(GREEN);
	
	//Spiel starten
	running = 1;
	start_game = 0;
	
	// verbleibende Leben senden um spielstart anzuzeigen
	buf[1] = '0' + life_left;
	sendPacket(2, 0, buf, 2);
	buf[1] = '0' + life_right;
	sendPacket(3, 0, buf, 2);
}

