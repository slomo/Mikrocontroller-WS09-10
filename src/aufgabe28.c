// Standart Headerfiles siehe aufgabe1.c
#include "aufgabe28.h"

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED					(0x01)
#define YELLOW				(0x02)
#define GREEN				(0x04)
#define LED_OFF(led)	    (P4OUT |= led)    
#define LED_ON(led)      	(P4OUT &= ~led)    
#define LED_TOGGLE(led)  	(P4OUT ^=  led)

// Variable gibt an ob eine Ausgabe gemacht werden soll
volatile int do_output=0;

// Funktion welche die Daten eines Packet is dest schreibt
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

    // Arbeitspalznummer +10
	setUid(17);
    // Selbe fequenz wie unsere Nachbarn mit denen wir komunizieren wollen
	switchFreq(8);
	while(1) {
		if(do_output==1) {
            // Daten des Packets holen und vorbereiten
			sprintPacket(str);
			len=strlen(str);
			writestr(str);
            
            // Code welcher die Funtionalitaet folgender Zeile hat
            // sscanf(j,"%d\t%d\t%d\r\n",NULL,NULL,ax,ay,az,knopp);
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
			
            // Je nach dem welcher Knopf gedrueckt wurde LEDs einschalten
			if(knopp==2){
				LED_ON(GREEN);
			}
			if(knopp==1){
				LED_OFF(GREEN);
			}

            // Entscheiden ob einer der Beschleunigungswerte mehr als
            // 20 % vom alten abweicht - wenn ja rote LED einschalten
			if(lastax!=-1){
				
				if(	abs(lastay-ay) >= lastay*0.2 || 
					abs(lastax-ax) >= lastax*0.2 || 
					abs(lastaz-az) >= lastaz*0.2 ){
					LED_ON(RED);
				}else{
					LED_OFF(RED);
				}
					
			}
            // alte Werte fuer den naechten Druchlauf setzen
			lastax=ax;
			lastay=ay;
			lastaz=az;

            // gelbe LED als Statusanzeige toggeln
			LED_TOGGLE(YELLOW);
			do_output=0;
			
            // ACK an den sender schicken
			sprintf(str,"FACK:%d",len);
			sendPacket(18,17,str,strlen(str));
		}
	}
	
}
