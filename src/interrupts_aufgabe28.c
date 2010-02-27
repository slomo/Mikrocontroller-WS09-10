#include "msp430x16x.h"
#include "interrupts.h"
#include "system.h"
#include "CC1100.h"
#include "stdio.h"
#include "string.h"
#include "aufgabe28.h"

	
//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
//______Interruptquelle_______________INTNR_|_ADR____|_Funktion_________|
PORT2_ISR(ISR_Port2)             	//int01 | 0xFFE2 | Port2			|

__interrupt void ISR_Port2 (void) {
	char res ;			// CRC Check 
	if (P2IFG & 0x01)	// Check P2IFG Bit P2.0 - CC1100 Rx Packet
		{ 
		CLEAR(P2IFG, 0x01);
		res = receivePacket(); 	// CRC Rückgabe 
		if (res)				// wenn Packet OK ...
			{
				// wenn ein Packet Empfangen wurde die Ausgabe
                // einleiten
				do_output=1;
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
