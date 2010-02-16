#include "msp430x16x.h"		// Systemdefinitionen von TI für den MSP430F1612
#include "init.h"			// Initialisierung des Mikrocontrollers
#include "CC1100.h"			// CC1100 Funktransceiver
#include "system.h"			// Systemfunktionen MSB430H
#include "interrupts.h"		// ISR - Interrupt Service Routinen
#include "stdio.h"			// includes TI MSP430F1612 
#include "SHT11.h"			// SHT11 Temperatur- und Feuchtesensor
#include "aufgabe28.h"

#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

volatile int do_output=0;

void sprintPacket(char* dest) {
	int i;
	for (i=2;i<((RxCC1100.length));i++) {
			dest[i-2]=RxCC1100.data[i];
	}
	dest[RxCC1100.length-2]=0;
}

void aufgabe28() {
	char str[255];
	char *i,*j=str;
	int len;
	int knopp=0,ax=0,ay=0,az=0;
	
	int lastax=-1;
	int lastay=-1;
	int lastaz=-1;

	setUid(17); //Arbeitspalznummer +10
	switchFreq(8); //Selbe fequenz wie unsere nachbarn mit denen wir komunizieren wollen
	while(1) {
		if(do_output==1) {
			
			sprintPacket(str);
			len=strlen(str);
			writestr(str);
			
			j=str;
			while(*j!='\t') j++; j++;
			while(*j!='\t') j++;
			i=++j;
			while(*j!='\t') j++; *j='\0';
			ax=atoi(i);
			i=++j;
			while(*j!='\t') j++; *j='\0';
			ay=atoi(i);
			i=++j;
			while(*j!='\t') j++; *j='\0';
			az=atoi(i);
			i=++j;
			while(*j!='\r') j++; *j='\0';
			knopp=atoi(i);
			
			//sscanf(j,"%d\t%d\t%d\r\n",NULL,NULL,ax,ay,az,knopp);
			
			
			if(knopp==2){
				LED_ON(GREEN);
			}
			if(knopp==1){
				LED_OFF(GREEN);
			}
			
			if(lastax!=-1){
				
				if(	abs(lastay-ay) >= lastay*0.2 || 
					abs(lastax-ax) >= lastax*0.2 || 
					abs(lastaz-az) >= lastaz*0.2 ){
					LED_ON(RED);
				}else{
					LED_OFF(RED);
				}
					
			}
			lastax=ax;
			lastay=ay;
			lastaz=az;

			LED_TOGGLE(YELLOW);
			do_output=0;
			
			sprintf(str,"FACK:%d",len);
			sendPacket(18,17,str,strlen(str));
		}
	}
	
}
