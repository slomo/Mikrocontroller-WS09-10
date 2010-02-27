#include "msp430x16x.h"
#include "interrupts.h"
#include "system.h"
#include "CC1100.h"
#include "stdio.h"
#include "string.h"
#include "aufgabe25.h"

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
TIMERB0_ISR(ISR_Timer_B_CCR0)     //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_Timer_B_CCR0 (void) {
	// Bei jedem Interupt den naechsten Wert in den DAC schreiben
	DAC12_1DAT=values[i];
	i++;
	i=i%100;
	TBCCTL0 &= ~CCIFG; 
}	
