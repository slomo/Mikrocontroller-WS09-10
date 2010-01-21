#include "msp430x16x.h"        // Systemdefinitionen von TI für den MSP430F1612
#include "init.h"            // Initialisierung des Mikrocontrollers
#include "CC1100.h"            // CC1100 Funktransceiver
#include "system.h"            // Systemfunktionen MSB430H
#include "interrupts.h"        // ISR - Interrupt Service Routinen
#include "stdio.h"            // includes TI MSP430F1612 
#include "SHT11.h"            // SHT11 Temperatur- und Feuchtesensor
#include "project.h"
#include "math.h"


int valuesX[ANTZ];
int valuesY[ANTZ];

int i;

volatile int ic=2;

barstate left_bar;
barstate right_bar;
int life_left;
int life_right;
int running;
ballstate ball;

#define MAX(s1,s2) ((s1 > s2) ? s1 : s2) 
#define MIN(s1,s2) ((s1 < s2) ? s1 : s2)
#define GRAD(g) (g / 360.0 * 2.0 * M_PI)

void project(){
    //DMA vorbereiten
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
    
    //DAC vorbereiten
    DAC12_1CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1 + DAC12ENC + DAC12LSEL0;
    DAC12_0CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1 + DAC12ENC + DAC12LSEL0;
    
    //Timer und output
    //P6SEL = 0xc0;
    TBCTL     = MC_1 + TASSEL_2 + ID_3;
    //TBCCTL0 = CCIE; 
    TBCCR0     = 100;

	P1IE |= 0x03;
	P1IES &= ~0x03;
    
	// Beschleunigungssensor als Randomquelle
	P6SEL = 0x01;
	ADC12MCTL0 = SREF_0 + INCH_0 + EOS;
	ADC12CTL1 = CONSEQ_1 + SHP;
	ADC12CTL0 = ADC12ON + ENC + ADC12SC + MSC;
	
	P5DIR |= (64+32+16);
	P5OUT &= ~(32 + 16); //GS1 und GS2 (empfindlichkeit) einstellen
	P5OUT |= 64;			//Wake up, wake up, wake up now... so tierd of wa~i~ting ...*sing*

	ADC12IE = 0x01; // AD-Wandler Intterrup enable
	_bis_SR_register(GIE); //Interrupts zulassen

	ADC12CTL0 |=  ADC12SC;
	ADC12CTL0 &= ~ADC12SC;    
    
    DAC12_1DAT=valuesY[0];
    DAC12_0DAT=valuesX[0];


	// here be pong
    init(&ball);
  	   	
    while(1) {
    	int dir;
   	
    	if (running) {
	        if((dir = next(&ball, left_bar, right_bar)) != IN) {
	        	if (life_left < 0 || life_right < 0) {
	        		running = 0;
	        	}
	        	else {
	    	    	reset_ball(&ball, dir);
	        	}
        	}

	        left_bar = MAX(0 + BARLENGTH/2.0, MIN(FIELD_Y - BARLENGTH/2.0, ball.y));
	        LED_TOGGLE(GREEN);
	        generate_array(&ball, left_bar, right_bar);
    	}
    }    
}

int next(ballstate *ball, barstate bar_left, barstate bar_right)
{
        // move ball
        float m_x, m_y;
        
        // calulcate the movement vectors
        m_x = ball->speed * cos(ball->angle);
        m_y = ball->speed * sin(ball->angle);
        
        // add movement vectros to current position
        ball->x += m_x;
        ball->y += m_y;
        
        // check for collisions
        // ball has reached the borders
        if ((ball->y <= 0) || (ball->y >= FIELD_Y)) {
            ball->angle = 2*M_PI - ball->angle;
            
            if (ball->y >= FIELD_Y) {
                // ball crossed border at yMax
                ball->y = 2 * FIELD_Y - ball->y;
            }
            else {
                // ball crossed border at yMin
                ball->y = fabs(ball->y);
            }
            
            ball->speed += 0.2;
            LED_TOGGLE(YELLOW);
        }
        
        // ball has passed bar layer
        if(ball->x <= 0 || ball->x >= FIELD_X){

            float bar_position;
            
            if (ball->x <= 0) {
                // calculate if 
	            float collision_point_y = fabs(tan(ball->angle) * (ball->x - m_x) + (ball->y - m_y));
                bar_position = bar_left - collision_point_y;

                // ball crossed border at xMin
                ball->x = fabs(ball->x);
            }
            else {
                // calculate if 
	            float collision_point_y = fabs(tan(ball->angle) * (FIELD_X - ball->x - m_x) + (ball->y - m_y));
                bar_position = bar_right - collision_point_y;

                // ball crossed border at xMax
                ball->x = 2 * FIELD_X - ball->x;
            }

            if (fabs(bar_position) <= (BARLENGTH/2.0)) {
                ball->angle = (M_PI - ball->angle) + (bar_position / (BARLENGTH/2.0)) * GRAD(30);
				
				// Winkel nicht zu steil
				if (ball->angle > GRAD(70) && ball->angle < GRAD(110)) {
					if (ball->angle > GRAD(90)) {
						ball->angle = GRAD(110);
					}
					else {
						ball->angle = GRAD(70);
					}
				}
				else if (ball->angle > GRAD(250) && ball->angle < GRAD(290)) {
					if (ball->angle > GRAD(270)) {
						ball->angle = GRAD(290);
					}
					else {
						ball->angle = GRAD(250);
					}
				}
								
                //Use the speedup feature later when averything works
                ball->speed += 0.5;
                LED_TOGGLE(RED);
                return IN;
            }
            
            if (ball->x <= 0) {
            	life_left--;
	            return LEFT;
            }

            life_right--;
            return RIGHT;
        }

        return IN;
}

void generate_array(ballstate *ball, barstate bar_left, barstate bar_right)
{
    int i, count;
    for(i=0;i<BALL_POINTS;i++){
        valuesX[i] = (int)(ball->x * RES_X/FIELD_X);
        valuesY[i] = (int)(ball->y * RES_Y/FIELD_Y);
    }
    
    count = ((ANTZ-BALL_POINTS) / 2);
    for (i = 1; i < count + 1; i++) {
        valuesX[i + BALL_POINTS-1] = 0;
        valuesY[i + BALL_POINTS-1] = (int)(((bar_left - BARLENGTH/2.0) + (BARLENGTH / count) * i) * RES_Y/FIELD_Y);
    }
    
    
    for (i = count + 1; i <= ANTZ - BALL_POINTS; i++) {
        valuesX[i + BALL_POINTS-1] = (int)RES_X;
        valuesY[i + BALL_POINTS-1] = (int)(((bar_right - BARLENGTH/2.0) + (BARLENGTH / count) * (i-count)) * RES_Y/FIELD_Y);
    }
}

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

void reset_ball(ballstate *ball, int direction)
{
    ball->x = FIELD_X / 2 + (FIELD_X / 4.0 * -1.0 * direction);
    ball->y = FIELD_Y / 2;
    ball->speed = 2;
    
    if (direction < 0) {
    	if (irand(2) == 0) {
    		ball->angle = GRAD(180 - irand(45));
    	}
    	else {
    		ball->angle = GRAD(180 + irand(45));
    	}
    }
    else {
    	if (irand(2) == 0) {
    		ball->angle = GRAD(360 - irand(45));
    	}
    	else {
    		ball->angle = GRAD(irand(45));
    	}
    }
}

