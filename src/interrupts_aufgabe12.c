#include "msp430x16x.h" // Systemdefinitionen von TI fur den MSP430F1612
#include "interrupts.h" // ISR - Interrupt Service Routinen
#include "system.h"     // Systemfunktionen MSB430H
#include "CC1100.h"     // CC1100 Funktransceiver
#include "stdio.h"      // includes TI MSP430F1612 


//Zuweisung der Interruptvektoren auf die ISR Behandlungsroutinen
PORT1_ISR(ISR_Port1)

// Zaehler fuer die Anzahl der Tasterbetaetigungen
int i=0;

__interrupt void ISR_Port1 (void) { 
    switch (P1IFG & 0x03) {
        case 0x01 :  // rechter Taster gedrueckt
            // Zaehler fuer Tasterbetaetigungen erhoehen
            i++;
            // wenn der Zaehler 10 erreichet, gruene LED toggeln und
            // Zahler auf 0 zuruecksetzen
            if (i == 10) {
                P4OUT ^= 0x04;
                i=0;
            }
            // kurz warten um eventuelles Prellverhalten des Tasters
            // abzufangen
            wait(100);
        break;
        case 0x02 : // linker Taster gedruckt
            // gelbe LED toggeln
            P4OUT ^= 0x02;
            // Interupts fuer den Taster an Port1.0 toggeln
            P1IE ^= 0x01;
            // kurz warten um eventuelles Prellverhalten des Tasters
            // abzufangen
            wait(100);
        break;

    }

    // Alle Interuptflags für Port 1 entfernen um eine Wiederholung
    // der ISR, wegen gesetzem Interuptfalg, zu verhindern
    CLEAR(P1IFG, 0xFF);
   
}
