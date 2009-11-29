#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe23.h"

#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

int do_a_barrel_roll = 0;

void aufgabe23() {
	char str[255];
	double voltage;
	//
	P6SEL = 0x10;
	ADC12MCTL0 = SREF_0 + INCH_4;
	ADC12CTL1 = CONSEQ_0 + SHS_0;
	ADC12CTL0 = ADC12ON;

	//Optionen zur Sekuendlichen ausloesung eines timerinterupts
	TBCTL 	= MC_1 + TASSEL_1 + ID0 + ID1;
	TBCCTL0 = CCIE; 
	TBCCR0 	= 4000;
	_bis_SR_register(GIE); //Interrupts zulassen

	while(1){
		if (do_a_barrel_roll == 1) {
			//ENC Toggle um eine Wandlung zu bekommen
			ADC12CTL0 = ADC12ON + ENC + ADC12SC;
			while (ADC12CTL1 & ADC12BUSY) {
				ADC12CTL0 &= ~ADC12SC;
			}
			
			voltage = ADC12MEM0*3.0/4096;
			sprintf(str,"U= %.2f V\n",voltage);
			
			if(voltage <= 1){
				LED_ON(YELLOW);
				LED_OFF(GREEN);
				LED_OFF(RED);
			}
			if(voltage <= 2 && voltage > 1 ){
				LED_OFF(YELLOW);
				LED_ON(GREEN);
				LED_OFF(RED);
			}
			if(voltage > 2){
				LED_OFF(YELLOW);
				LED_OFF(GREEN);
				LED_ON(RED);
			}
			
			ADC12CTL0 &= ~ENC;
			writestr(str);
			do_a_barrel_roll = 0;
		}
	}
}
