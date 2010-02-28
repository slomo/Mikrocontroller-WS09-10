#include "msp430x16x.h"
#include "interrupts.h"
#include "../system.h"
#include "../CC1100.h"
#include "stdio.h"
#include "string.h"
#include "project.h"

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
//______Interruptquelle_______________INTNR_|_ADR____|_Funktion
PORT2_ISR(ISR_Port2)             	//int01 | 0xFFE2 | Port2
PORT1_ISR(ISR_Port1)                //int04 | 0xFFE8 | Port1
ADC12_ISR(ISR_ADC12)             	//int07 | 0xFFEE | ADC

__interrupt void ISR_Port2 (void) {
	char res ;			// CRC Check 
	if (P2IFG & 0x01)	// Check P2IFG Bit P2.0 - CC1100 Rx Packet
		{ 
		CLEAR(P2IFG, 0x01);
		res = receivePacket(); 	// CRC Rückgabe 
		if (res)				// wenn Packet OK ...
			{
				// Wenn noch keine 2 Spieler da sind verarbeite
				// ready anfragen von Spielern
				if (player < 2) {
					// R = READY
					if (RxCC1100.data[2] == 'R') {
						char buf[3];
						
						// Wenn ein ready Packet empfangen wird,
						// Spieler im System vermerken und ihm seine
						// ID als C - Antwort schicken
						player++;
						buf[0] = 'C';
						buf[1] = '1' + player;
						buf[2] = 0;
						sendPacket(0, 1, buf, 2);
						if (player == 2) {
							start_game = 1;
						}
					}
				}
				// Wenn bereits 2 Spieler vorhanden sind, gehoeren 
				// ankommende Packete zu Spielsteuerung
				else {
					// P = POSITION
					if (RxCC1100.data[2] == 'P') {
						// ankommende Daten Parsen und die uebertragenen
						// Werte auf den Schlaeger des jeweiligen
						// Spielers uebertragen
						int value;
						value = (RxCC1100.data[3] - '0');
						
						if (RxCC1100.length > 3) {
							value *= 10;
							value += (RxCC1100.data[4] - '0');
							
							if (RxCC1100.length > 4) {
								value *= 10;
								value += (RxCC1100.data[5] - '0');
							}
						}
							
						if (RxCC1100.source == 3) {
							// schläger verschieben
							left_bar = (FIELD_Y/400.0)*(float)value;
						}
						else if (RxCC1100.source == 2) {
							//schläger verschieben
							right_bar = (FIELD_Y/400.0)*(float)value;
						}
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
	// Spiel starten wenn beide Spieler da sind und Taster gedrückt wird
	if (player == 2) {
		start_game = 1;
	}
  	CLEAR(P1IFG, 0xFF); // Clear all flags
}

__interrupt void ISR_ADC12 (void)
{
	if(ADC12IFG & 0x01){
		srand(ADC12MEM0);
		ADC12MCTL0 = 0;
		ADC12CTL1 = 0;
		ADC12CTL0 = 0;
		P5OUT &= ~64;
	}
}
