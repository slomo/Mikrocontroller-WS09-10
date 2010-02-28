// alle StandartHeaderfiles (siehe Aufgabe 12) einbinden
#include "aufgabe24.h"

//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
ADC12_ISR(ISR_ADC12)            //int07 | 0xFFEE | ADC
TIMERB0_ISR(ISR_Timer_B_CCR0)   //int13 | 0xFFFA | Timer B CC0

__interrupt void ISR_ADC12 (void) {
    // Wenn die Wandlung fertig ist Wert ausgeben
    if(ADC12IFG & 0x04) {
        sprintf(str,"%d %d %d\n",ADC12MEM0,ADC12MEM1,ADC12MEM2);
        writestr(str);
    }
}

__interrupt void ISR_Timer_B_CCR0 (void) {
    // Wenn eine Sekunde vergangen ist Wandlung ausloesen
    do_a_barrel_roll = 1;
    
    TBCCTL0 &= ~CCIFG; 
}
