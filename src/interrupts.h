#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

// Festlegung der verfügbaren Interruptroutinen
// bei Bedarf die Auskommentierung entfernen 
//===========================================================
//__interrupt void ISR_DACDMA (void);           // Routine: DMA und DAC
//__interrupt void ISR_Port2 (void);            	// Routine: Port2
//__interrupt void ISR_USART1_Transmit (void);  // Routine: USART1 Versand
//__interrupt void ISR_USART1_Receive (void);   // Routine: USART1 Empfang
__interrupt void ISR_Port1 (void);            // Routine: Port1
//__interrupt void ISR_Timer_A (void);          // Routine: TimerA CC1-2,TA
//__interrupt void ISR_Timer_A_CCR0 (void);     // Routine: Timer_A CC0
__interrupt void ISR_ADC12 (void);            // Routine: 12Bit ADC
//__interrupt void ISR_USART0_Transmit (void);  // Routine: USART0 Versand
//__interrupt void ISR_USART0_Receive (void);   // Routine: USART0 Empfang
//__interrupt void ISR_Watchdog (void);         // Routine: Watchdog Timer
//__interrupt void ISR_Comparator_A (void);     // Routine: Comparator_A
//__interrupt void ISR_Timer_B (void);          // Routine: Timer B CC1-6,TB
//__interrupt void ISR_Timer_B_CCR0 (void);     // Routine: Timer_B CC0
//__interrupt void ISR_NMI(void);				// Routine: Non-maskable Int
//===========================================================
//

#endif /*INTERRUPTS_H_*/
