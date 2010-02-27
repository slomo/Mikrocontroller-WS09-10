#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

// Festlegung der verfügbaren Interruptroutinen
__interrupt void ISR_Port2 (void);            // Routine: Port2
__interrupt void ISR_Port1 (void);            // Routine: Port1
__interrupt void ISR_ADC12 (void);            // Routine: 12Bit ADC

#endif /*INTERRUPTS_H_*/
