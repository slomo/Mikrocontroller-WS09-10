#include "msp430x16x.h"        // Systemdefinitionen von TI für den MSP430F1612
#include "../init.h"            // Initialisierung des Mikrocontrollers
#include "../CC1100.h"            // CC1100 Funktransceiver
#include "../system.h"            // Systemfunktionen MSB430H
#include "interrupts.h"        // ISR - Interrupt Service Routinen
#include "stdio.h"            // includes TI MSP430F1612 
#include "../SHT11.h"            // SHT11 Temperatur- und Feuchtesensor
#include "project.h"

int ready;
int leben;

void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void project(){
	ready = 0;
	
	// Funk vorbereiten
	initUART0_SPI();
	initCC1100();
	setUid(1);
	switchFreq(7);
	
	P1IE |= 0x03;
	P1IES &= ~0x03;
	
  P6SEL = 0x07;
  
  // 16 Werte automatisch mit dem ADC wandeln
  ADC12MCTL0 = INCH_0;
  ADC12MCTL1 = INCH_1;
  ADC12MCTL2 = INCH_2;
  ADC12MCTL3 = INCH_0;
  ADC12MCTL4 = INCH_1;
  ADC12MCTL5 = INCH_2;
  ADC12MCTL6 = INCH_0;
  ADC12MCTL7 = INCH_1;
  ADC12MCTL8 = INCH_2;
  ADC12MCTL9 = INCH_0;
  ADC12MCTL10 = INCH_1;
  ADC12MCTL11 = INCH_2;
  ADC12MCTL12 = INCH_0;
  ADC12MCTL13 = INCH_1;
  ADC12MCTL14 = INCH_2;
  ADC12MCTL15 = INCH_3 + EOS;
  
  ADC12CTL1 = SHP + CONSEQ0;
  ADC12CTL0 = ADC12ON + ENC + ADC12SC + MSC + SHT0_15;
 
  P5DIR |= (0x64+0x32+0x16);
  P5OUT &= ~(0x32 + 0x16); //GS1 und GS2 (empfindlichkeit) einstellen
  P5OUT |= 0x64; //Wake up, wake up, wake up now... so tierd of wa~i~ting ...*sing*
 
  //Optionen zur halb-sekuendlichen ausloesung eines timerinterupts
  TBCTL = MC_1 + TASSEL_1 + ID0 + ID1;
  TBCCR0 = 200;
 
  ADC12IE = 0x01 + 0x02 + 0x04; // AD-Wandler Intterrup enable
  _bis_SR_register(GIE); //Interrupts zulassen

	LED_OFF(RED);
	LED_OFF(YELLOW);
	LED_OFF(GREEN);
	
	while (1) {
    if (ready == 3) {
       TBCCTL0 |= CCIE;
  
       LED_OFF(RED);
       LED_OFF(YELLOW);
       LED_OFF(GREEN);
       
       switch (leben) {
          case 3:
            LED_ON(GREEN);
          case 2:
            LED_ON(YELLOW);
          case 1:
            LED_ON(RED);
       }
    }
    else {
       TBCCTL0 &= ~CCIE;
  
    	if (ready == 2) {
  			LED_ON(RED);
  			delay(350);
  			LED_OFF(RED);
  			LED_ON(YELLOW);
  			delay(350);
  			LED_OFF(YELLOW);
  			LED_ON(GREEN);
  			delay(350);
  			LED_OFF(GREEN);
  			LED_ON(YELLOW);
  			delay(350);
  			LED_OFF(YELLOW);
  			LED_ON(RED);
  			delay(350);
  		}
  		else if (ready) {
  			LED_ON(RED);
  			LED_ON(YELLOW);
  			LED_ON(GREEN);
  			delay(500);
  			LED_OFF(RED);
  			LED_OFF(YELLOW);
  			LED_OFF(GREEN);
  			delay(500);
  		}
    }
	}
}	

