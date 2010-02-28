// Standart Headerfiles siehe aufgabe1.c
#include <math.h>
#include "aufgabe28.h"
 
// interrupt.h /.c
TIMERB0_ISR(Aufgabe28_ISR_Timer_B_CCR0)
ADC12_ISR(Aufgabe28_ISR_ADC12)
PORT2_ISR(Aufgabe28_ISR_Port2)
 
char aufgabe28_output[255];
int aufgabe28_saved_len;
 
 
void Aufgabe28_Init(void) {
    setUid(18);
       switchFreq(8);
    
    // 5s ACLK
    TBCCR0 = 2000;
    
    // ALCK als Interruptquelle, UP Mode, Interrupt aktivieren
    TBCTL &= ~(TBSSEL1 | MC1);
    TBCTL |= TBSSEL0 | MC0 | ID1 | ID0;
       
    // Interrupt aktivieren
    TBCCTL0 |= CCIE;
    
    // General Interrupt Enable
    _bis_SR_register(GIE);
    
    // P6SEL5 für AD-Wandler setzen
    P6SEL |= 0x7;
    
    // setze defaults
    ADC12CTL0 = 0;
    ADC12CTL1 = 0;
    
    // aktivieren
    ADC12CTL0 |= ADC12ON | MSC;
    ADC12CTL1 |= SHP | CONSEQ0;
 
    // Eingang wählen und Referenzspannung setzen
    ADC12MCTL0 = SREF_0 | INCH_0;
    ADC12MCTL1 = SREF_0 | INCH_1;
    ADC12MCTL2 = EOS | SREF_0 | INCH_2;
 
    // enable conversion
    ADC12CTL0 |= ENC;
 
    // ADC Interrupt aktivieren
    ADC12IE |= 0x7;
       
    // 1.5g-Messbereich
    P5OUT &= ~(0x10 | 0x20);
    P5OUT |= 0x40;
}
 
void Aufgabe28(void) {
       
}
 
__interrupt void Aufgabe28_ISR_Timer_B_CCR0 (void) {
    // sample starten
    ADC12CTL0 |= ADC12SC;
 
    // sample abschließen
    ADC12CTL0 &= ~ADC12SC;
    
    // Timer_B control - Löst ein Timer_B clear aus
    TBCTL |= TBCLR;
    // verhindert das nochmalige Aufrufen der Interruptfunktion
    TBCCTL0 &= ~CCIFG;
}
 
__interrupt void Aufgabe28_ISR_ADC12 (void) {
    SHT11_Read_Sensor();
 
    sprintf(aufgabe28_output, "%s\t%s\t%d\t%d\t%d\t%d\r\n", temp_char,
            humi_char, ADC12MEM0, ADC12MEM1, ADC12MEM2, (P1IN & 0x3));
    
    P4OUT &= ~ROT;
    aufgabe28_saved_len = strlen(aufgabe28_output);
    sendPacket(17, 18, aufgabe28_output, aufgabe28_saved_len);
}
 
__interrupt void Aufgabe28_ISR_Port2 (void) {
    // weitesgehend aus der gegebenen CC1100.c übernommen
    char res;
    char *checksum;
    int len, i;
       
    // Check P2IFG Bit P2.0 - CC1100 Rx Packet
    if (P2IFG & 0x01) {
        CLEAR(P2IFG, 0x01);
        res = receivePacket(); // CRC Rückgabe
            if (res) { // wenn Packet OK ...
                // empfangene Paketdaten kopieren (RxCC1100.data ist
                // volotile und kann in keine Funktionen verwendet
                // werden)
                for (i=2;i<((RxCC1100.length));i++) {
                    aufgabe28_output[i-2] = RxCC1100.data[i];
                }                      
                aufgabe28_output[RxCC1100.length - 1] = '\0';
                       
                if (memcmp(aufgabe28_output, "FACK:", 5) == 0) {
                    // wenn Inhalt des Paketes mit FACK: anfängt,
                    // Zahl dahinter auswerten
                    checksum = aufgabe28_output + 5;
                    len = atoi(checksum);
                               
                    if (len == aufgabe28_saved_len) {
                        // wenn zurückgesendete Zahl mit der
                        // ursprünglichen Länge übereinstimmt
                        // wird angenommen, dass das Paket korrekt
                        // übermittelt wurde
                        P4OUT |= ROT;
                    }
                }
            }
            else {      
                spiStrobe(CC1100_SIDLE); // Switch to IDLE
                spiStrobe(CC1100_SFRX);  // Flush the RX FIFO
            }
    }      
    else {
        CLEAR(P2IFG, 0xFF); // Clear all flags
    }
    spiStrobe(CC1100_SRX); // Switch to RX Mode
}

