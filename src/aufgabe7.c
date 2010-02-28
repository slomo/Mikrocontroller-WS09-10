// Standart Headerfiles siehe aufgabe1.c

void aufgabe7() {
		switch (P1IN & 0x03) {
			case 0x01 :  // rechter Taster gedruckt
                // Taktrate 4,096kHz einstellen (niedriger Stromverbrauch)
				DCOCTL 	= 0x0;  // 000 00000
                                // alle takterhoehenden Register auf 0
                                
				BCSCTL1 = 0x80;	// 1 0 00 0 000
                                // XT2CLK wie vorgegeben deaktivieren
                                
				BCSCTL2 = 0xf1;	// 11 11 0 00 1
                                // LFXT1CLK als Quelle fuer MCLK
                                // Teiler fuer MCLK auf 8 setzen
                                // externen DCO Wiederstand waehlen
			break;
			case 0x02 : // linker Taster gedruckt
                // Taktrate 7,3728 MHz einstellen (hoeherer Stromverbrauch)
				DCOCTL 	= 0xbe; // 1011 1110
                                // DCO5 waehlen
                                // MOD14 waehlen
                                
				BCSCTL1 = 0x85; // 1 0 00 0 101
                                // XT2CLK wie vorgegeben deaktivieren
                                // RSEL5 waehlen
                                
				BCSCTL2 = 0x1;  // 00 00 0 00 1
                                // externen DCO Wiederstand waehlen
			break;
			
			default :
		}
}
