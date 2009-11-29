#include "msp430x16x.h"		// Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "string.h"
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe21.h"

#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

struct time t1,t2,now;

int timecmp(struct time *s1, struct time *s2) {
	return ((s1->s == s2->s) && (s1->ss == s2->ss) && (s1->mm == s2->mm) && (s1->hh == s2->hh));
}

void output_str_on_serial(char* str, int len) {
	int i = 0;
	while((str[i] != '\0') && (len > i)){	
		if (U1TCTL & TXEPT) { //Wenn sendebuffer leer
			U1TXBUF = str[i];
			i++;
		}
	}
}

void output_data() {
	char str[255];
	SHT11_Read_Sensor();
	sprintf(str,"%d:%d:%d.%d - Temp: %s Hum: %s\r\n",now.hh,now.mm,now.ss,now.s,temp_char,humi_char);
	output_str_on_serial(str,255);
}

void aufgabe21() {
	now.hh=0;
	now.mm=0;
	now.ss=0;
	now.s=0;
	
	t1.hh=0;
	t1.mm=0;
	t1.ss=5;
	t1.s=0;
	
	t2.hh=0;
	t2.mm=0;
	t2.ss=10;
	t2.s=0;
	
	TBCTL 	= MC_1 + TASSEL_1 + ID0 + ID1;
	TBCCTL0 = CCIE; 
	TBCCR0 	= 400;
	_bis_SR_register(GIE); //Interrupts zulassen	
	while(1){	
	}
}
