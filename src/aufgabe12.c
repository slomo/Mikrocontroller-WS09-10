#include "msp430x16x.h" // Systemdefinitionen von TI fur den MSP430F1612
#include "init.h"       // Initialisierung des Mikrocontrollers
#include "CC1100.h"     // CC1100 Funktransceiver
#include "system.h"     // Systemfunktionen MSB430H
#include "interrupts.h" // ISR - Interrupt Service Routinen
#include "stdio.h"      // includes TI MSP430F1612
#include "SHT11.h"      // SHT11 Temperatur- und Feuchtesensor

// Definitionen um das Ein- und Ausschalten von LEDs zu erleichtern
#define RED             (0x01)
#define LED_OFF(led)    (P4OUT |= led)
#define LED_ON(led)     (P4OUT &= ~led)

// Funtion, welche die bereits vorhandene Funktion wait verwendet
// um den Programmfluss zu um n Millisekunden zu verzoegern
void delay(unsigned int time_mill) {
    unsigned int i;
    for(i=0;i<=time_mill;++i){
        wait(100);
    }
}

void aufgabe12() {
    // Interrupts erlauben
    _bis_SR_register(GIE);
    // Interrupts fuer den Taster an Port 1.1 aktivieren
    P1IE |= 0x02;
    while(1){
        // Rote LED alle 500ms aus bzw. einschalten
        delay(500);
        LED_OFF(RED);
        delay(500);
        LED_ON(RED);
    }
}
