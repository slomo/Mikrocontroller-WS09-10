// Standart Headerfiles siehe aufgabe1.c

void aufgabe8() {
    // Pin bei jedem Scheifendurchlauf toggeln, um
    // die ungefaehre Taktfequenz messen zu koennen
	P4OUT ^= 0x01;
}
