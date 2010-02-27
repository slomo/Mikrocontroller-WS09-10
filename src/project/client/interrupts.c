#include "msp430x16x.h"
#include "interrupts.h"
#include "../system.h"
#include "../CC1100.h"
#include "stdio.h"
#include "string.h"
#include "project.h"
#include "math.h"

char buf[255];
int uid;

unsigned int xn = 2000;
unsigned int yn = 2000;
unsigned int zn = 2000;

volatile unsigned int ax = 0;
volatile unsigned int ay = 0;
volatile unsigned int az = 0;

volatile int vx = 0;
volatile int vy = 0;
volatile int vz = 0;
	
volatile double wx = 0;
volatile double wy = 0;

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
//______Interruptquelle_______________INTNR_|_ADR____|_Funktion_________|
PORT2_ISR(ISR_Port2)             	//int01 | 0xFFE2 | Port2			|
PORT1_ISR(ISR_Port1)                //int04 | 0xFFE8 | Port1 			|
ADC12_ISR(ISR_ADC12)             	//int07 | 0xFFEE | ADC				|
TIMERB0_ISR(ISR_Timer_B_CCR0)     //int13 | 0xFFFA | Timer B CC0 		|

__interrupt void ISR_Port2 (void) {
	char res ;			// CRC Check 
	if (P2IFG & 0x01)	// Check P2IFG Bit P2.0 - CC1100 Rx Packet
	{ 
		CLEAR(P2IFG, 0x01);
		res = receivePacket(); 	// CRC Rückgabe 
		if (res)				// wenn Packet OK ...
		{
			// Wollen wir spielen?
			if (ready == 1) {
				// W = WAITING_FOR_PLAYER
				if (RxCC1100.data[2] == 'W') {

					// Wenn ein Server nach Spielern sucht antworten
					// P = PLAYER_READY
					sendPacket(0, 1, "R", 1);
				}
				// Antwort mit ID vom server Parsen und uid entsprechend
				// setzen
				else if (RxCC1100.data[2] == 'C' && RxCC1100.length > 3 &&
					RxCC1100.data[3] >= '0' && RxCC1100.data[3] <= '9') {

		    		// C = CHANNEL_SET
					uid = RxCC1100.data[3] - '0';
					setUid(uid);
					ready = 2;
				}
      }
      // Es geht los sobald die ersten werze fuer Leben ankommen
      else if (ready >= 2) {
        if (RxCC1100.data[2] == 'L' && RxCC1100.length > 3 &&
          RxCC1100.data[3] >= '0' && RxCC1100.data[3] <= '9') {

		  // empfangene Mennge Leben ueernehmen
          // L = LIFE_COUNT
          leben = RxCC1100.data[3] - '0';
          ready = 3;
        }
			} 
		}
		else
		{	
			spiStrobe(CC1100_SIDLE); 	// Switch to IDLE
			spiStrobe(CC1100_SFRX);	 	// Flush the RX FIFO
		}
	}	
	else
	{
		CLEAR(P2IFG, 0xFF);			 	// Clear all flags
	}

	spiStrobe(CC1100_SRX);			 	// Switch to RX Mode
}

__interrupt void ISR_Port1 (void) {

	// Wenn der Knopf gedrueckt wird in Bereitschaftsmodus gehen
	if ((P1IFG & 0x03) && (ready < 1)) {
		ready = 1;
	}
	
  	CLEAR(P1IFG, 0xFF); // Clear all flags
}

__interrupt void ISR_ADC12 (void)
{
	// Durchschnitt von allen Wandungen nehmen und neigung Berechnen
	ax = ADC12MEM0 + ADC12MEM3 + ADC12MEM6 + ADC12MEM9 + ADC12MEM12;
	ay = ADC12MEM1 + ADC12MEM4 + ADC12MEM7 + ADC12MEM10 + ADC12MEM13;
    az = ADC12MEM2 + ADC12MEM5 + ADC12MEM8 + ADC12MEM11 + ADC12MEM14;
    vx = ax / 5 - xn;       
    vy = ay / 5 - yn;
    vz = az / 5 - zn;
    
    wx = atan2(vz, vx) * (400.0 / 3.0 /* == pi */);
    
    if (wx > 400 || wx < -200) {
    	wx = 400;
    }
    else if (wx < 0) {
  		wx = 0;
    }
    
    // Position an Server senden
    sprintf(buf,"P%0.3d", (int)wx);
    sendPacket(0, uid, buf, 5);
}

__interrupt void ISR_Timer_B_CCR0 (void) {
	// Neue Wandung Toggeln
	ADC12CTL0 |= ADC12SC;
  ADC12CTL0 &= ~ADC12SC;
}
