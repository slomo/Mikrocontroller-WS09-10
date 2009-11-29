#include "msp430x16x.h"
#include "interrupts.h"
#include "system.h"
#include "CC1100.h"
#include "stdio.h"
#include "string.h"
#include "aufgabe19.h"

	
//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
//Bei Bedarf fur den benutzten Interrupt die Auskommentierung entfernen 
//Die hochste Prioritat hat der int15

//______Interruptquelle_______________INTNR_|_ADR____|_Funktion_________|
//DACDMA_ISR(ISR_DACDMA)         	//int00 | 0xFFE0 | DAC12 / DMA		|
PORT2_ISR(ISR_Port2)             	//int01 | 0xFFE2 | Port2			|
//USART1TX_ISR(ISR_USART1_Transmit)	//int02 | 0xFFE4 | USART1 Transmit 	|
//USART1RX_ISR(ISR_USART1_Receive)  //int03 | 0xFFE6 | USART1 Receive	|
//PORT1_ISR(ISR_Port1)                //int04 | 0xFFE8 | Port1 			|
//TIMERA1_ISR(ISR_Timer_A)          //int05 | 0xFFEA | TimerA CC1-2,TA	|
//TIMERA0_ISR(ISR_Timer_A_CCR0)     //int06 | 0xFFEC | TimerA CC0 		|
//ADC12_ISR(ISR_ADC12)             	//int07 | 0xFFEE | ADC				|
//USART0TX_ISR(ISR_USART0_Transmit) //int08 | 0xFFF0 | USART0 Transmit	|
//USART0RX_ISR(ISR_USART0_Receive)  //int09 | 0xFFF2 | USART0 Receive	|
//WDT_ISR(ISR_Watchdog)             //int10 | 0xFFF4 | Watchdog Timer	|
//COMPARATORA_ISR(ISR_Comparator_A) //int11 | 0xFFF6 | Comparator A		|
//TIMERB1_ISR(ISR_Timer_B)          //int12 | 0xFFF8 | Timer B CC1-6,TB	|
TIMERB0_ISR(ISR_Timer_B_CCR0)     //int13 | 0xFFFA | Timer B CC0 		|
//=======================================================================
//NMI_ISR (ISR_NMI)					//int14 | 0xFFFC | Non-maskable		|						|
//									//int15 | 0xFFFE | Reset 			|
//=======================================================================
//

//==============================================================
// Implementierung der ISR Routinen 
// Achtung! Code in der ISR nicht zu lang machen,
// da sonst Timing oder Stack Probleme drohen.
// Fur die benotigten ISR die Auskommentierungen entfernen
// und eigenen Code fur die jeweilige ISR einfugen
//==============================================================


//==============================================================
//===INT:00====ADR:FFE0====DAC12 DMA ===========================
//==============================================================
//__interrupt void ISR_DACDMA (void) {
//  ...hier den Code der ISR einfugen
//}
//==============================================================

		
//==============================================================
//===INT:01====ADR:FFE2====PORT2================================
//==============================================================
__interrupt void ISR_Port2 (void) {
	char res ;			// CRC Check 
	if (P2IFG & 0x01)	// Check P2IFG Bit P2.0 - CC1100 Rx Packet
		{ 
		CLEAR(P2IFG, 0x01);
		res = receivePacket(); 	// CRC Ruckgabe 
		if (res)				// wenn Packet OK ...
			{
			printPacket();		// Packet auf Terminal ausgeben
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

//==============================================================
//===INT:02====ADR:FFE4====USART1 Transmit======================
//==============================================================
//__interrupt void Interruptroutine_USART1_Transmit (void) {
//  ...hier den Code der ISR einfugen
//}
//==============================================================


//==============================================================
//===INT:03====ADR:FFE6====USART1 Receive=======================
//==============================================================
//__interrupt void ISR_USART1_Receive (void)
//	{
//	unsigned char Zeichen;
//	IE2 &= ~URXIE1;			// Disable USART1 RX interrupt.
//	if (UART1writePointer == 32) UART1writePointer=0;
//	Zeichen = U1RXBUF;
//	UART1buffer[UART1writePointer++] = Zeichen;
//	UART1bufferLen++;
//  	if (Zeichen != '\n')
//  		{	
//		U1TXBUF = Zeichen; 	// Echo
//		} 
//	else
//		{
//		//writestr(&UART1buffer,strlen(UART1buffer));
//		UART1writePointer=0;
//		}				
//	IE2 |= URXIE1;    		// Enable USART1 RX interrupt. 
//	}
//==============================================================


//==============================================================
//===INT:04====ADR:FFE8====PORT1================================
//==============================================================

//__interrupt void ISR_Port1 (void) {
//  ...hier den Code der ISR einfugen	
//}
//==============================================================


//==============================================================
//===INT:05====ADR:FFEA====Timer_A Capture/Compare 1?2==========
//==============================================================
//__interrupt void ISR_Timer_A (void) {
//  ...hier den Code der ISR einfugen
// }
//==============================================================


//==============================================================
//===INT:06====ADR:FFEC****Timer_A Capture/Compare 0============
//==============================================================
//__interrupt void ISR_Timer_A_CCR0 (void) {
//  ...hier den Code der ISR einfugen
// }
//==============================================================


//==============================================================
//===INT:07====ADR:FFEE====ADC12================================
//==============================================================
//__interrupt void ISR_ADC12 (void)
//	{
//  ...hier den Code der ISR einfugen
//	}
//==============================================================


//==============================================================
//===INT:08====ADR:FFF0====USART0 Transmit======================
//==============================================================
//__interrupt void ISR_USART0_Transmit (void) {
//  ...hier den Code der ISR einfugen
// }
//==============================================================


//==============================================================
//===INT:9====ADR:FFF2====USART0 Empfang========================
//==============================================================
//__interrupt void ISR_USART0_Receive (void) {
//  ...hier den Code der ISR einfugen
// }
//==============================================================


//==============================================================
//===INT:10====ADR:FFF4====Watchdog Timer=======================
//==============================================================
//__interrupt void ISR_Watchdog (void) {
//  ...hier den Code der ISR einfugen
//}
//==============================================================


//==============================================================
//===INT:11====ADR:FFF6****Comparator_A=========================
//==============================================================
//__interrupt void ISR_Comparator_A (void) {
//  ...hier den Code der ISR einfugen
// }
//==============================================================


//==============================================================
//===INT:12====ADR:FFF8====Timer_B Capture/Compare 1?6==========
//==============================================================
//__interrupt void ISR_Timer_B (void) { 
//  ...hier den Code der ISR einfugen
//}
//==============================================================


//==============================================================
//===INT:13===ADR:FFFA====Timer_B Capture/Compare 0=============
//==============================================================
__interrupt void ISR_Timer_B_CCR0 (void) { 
	
	if (now.s==10) {
		now.s=0;
		now.ss++;
		P4OUT ^= 0x01;
		if (now.ss==60) {
			now.ss=0;
			now.mm++;
			if (now.mm==60) {
				now.mm=0;
				now.hh++;
				if(now.hh==24) {
					now.hh=0;
				}
			}
		}
	}
	else {
		now.s++;
	}
	if (timecmp(&now,&t1)) {
		P4OUT &= ~0x04;
	}
	
	if (timecmp(&now,&t2)) {
		P4OUT |= 0x04;
	}
	
	TBCCTL0 &= ~CCIFG;
//  ...hier den Code der ISR einfugen
}	
//===============================================================


//==============================================================
//===INT:14===ADR:FFFC====NMI ==================================
//==============================================================
//__interrupt void ISR_NMI (void) { 
//  ...hier den Code der ISR einfugen
//}	
//===============================================================
