// Standart Headerfiles siehe aufgabe1.c
#include "aufgabe24.h"

// Diese Variable gibt an, ob eine Wandlung am ADC vorgenommen wird
int do_a_barrel_roll = 0;

void aufgabe24() {
    // Variablen fuer die Beschleunigungswerte
    double ax,ay,az;

    // Portleitungen zum Sensor freigeben
    P6SEL = 0x07;
    
    // die noetigen Einstellungen am ADC vornehmen
    ADC12MCTL0 = SREF_0 + INCH_0;
    ADC12MCTL1 = SREF_0 + INCH_1;
    ADC12MCTL2 = SREF_0 + INCH_2 + EOS;
    ADC12CTL1 = CONSEQ_1 + SHP;
    ADC12CTL0 = ADC12ON + ENC + ADC12SC + MSC;
    
    // Zugang zu den eingaengen des Sensors freiben
    P5DIR |= (0x64+0x32+0x16);
    // GS1 und GS2 (empfindlichkeit) einstellen
    P5OUT &= ~(0x32 + 0x16);
    //Wake up senden um Sensor zu starten
    P5OUT |= 0x64;

    // Optionen zur Sekuendlichen ausloesung eines Timerinterupts
    TBCTL 	= MC_1 + TASSEL_1 + ID0 + ID1;
    TBCCTL0 = CCIE; 
    TBCCR0 	= 400;

    // AD-Wandler Interupt einschalten
    ADC12IE = 0x01 + 0x02 + 0x04;
    // Interrupts zulassen
    _bis_SR_register(GIE);

    while(1){
        if (do_a_barrel_roll == 1) {
            //ENC Toggle um eine Wandlung zu starten
            ADC12CTL0 |=  ADC12SC;
            ADC12CTL0 &= ~ADC12SC;

            // Variable fuer Wandlung zuruecksetzen
            do_a_barrel_roll = 0;
        }
    }
}
