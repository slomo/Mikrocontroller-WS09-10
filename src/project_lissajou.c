#define PI 3.1415926535897932384626433

#define SCALEX 1
#define SCALEY 1

int valuesX[2][ANTZ];
int valuesY[2][ANTZ];

int i;

volatile int ic=2;

float lissajous_x(float t, int a, float delta)
{
    return SCALEX * sin(a*t+delta)+1*SCALEX;
}

float lissajous_y(float t, int b)
{
    return SCALEY * sin(b*t)+1*SCALEY;
}

void delay(unsigned int time_mill) {	
	unsigned int i;
	for(i=0;i<=time_mill;++i){
		wait(100);
	}
}

void project(){
	
	float a=3,b=4,delta=0;
	int t,i=0;
	
	for(t=0;t<ANTZ;t++) {
    	valuesX[i][t]=(int) (lissajous_x((float)t*2.0*PI/((float)ANTZ),a,delta)*4096.0/3.0);
       	valuesY[i][t]=(int) (lissajous_y((float)t*2.0*PI/((float)ANTZ),b)*4096.0/3.0);
    }


	//DMA vorbereiten
	DMACTL0 = DMA0TSEL_8 + DMA1TSEL_8;
	DMACTL1 = ROUNDROBIN;
	
	DMA0CTL = DMASRCINCR_3 + DMADT_4 + DMAEN;
	DMA0SZ = ANTZ;
	DMA0SA = (unsigned int) valuesX;
	DMA0DA = (unsigned int) &DAC12_0DAT;
	
	DMA1CTL = DMASRCINCR_3 + DMADT_4 + DMAEN;
	DMA1SZ = ANTZ;
	DMA1SA = (unsigned int) valuesY;
	DMA1DA = (unsigned int) &DAC12_1DAT;
	
	//DAC vorbereiten
	DAC12_1CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1 + DAC12ENC + DAC12LSEL0;
	DAC12_0CTL = DAC12SREF0 + DAC12SREF1 + DAC12IR + DAC12AMP1 + DAC12ENC + DAC12LSEL0;
	
	//Timer und output
	//P6SEL = 0xc0;
	TBCTL 	= MC_1 + TASSEL_2 + ID_3;
	//TBCCTL0 = CCIE; 
	TBCCR0 	= 3;
	
	_bis_SR_register(GIE); //Interrupts zulassen
	
	DAC12_1DAT=valuesY[i][0];
	DAC12_0DAT=valuesX[i][0];
	while(1) {
		for(t=0;t<ANTZ;t++) {
    		valuesX[i][t]=(int) (lissajous_x((float)t*2.0*PI/((float)ANTZ),a,delta+((float)t*0.05/314.0))*4096.0/3.0);
        	//valuesY[t]=(int) (lissajous_y(t*0.1,b)*4096/3);
    	}
    	delta+=0.05;
    	if(delta>2*PI) {
    		delta=0;
    		LED_TOGGLE(GREEN);
    	}
    	LED_TOGGLE(RED);
    	
    	//i=(++i%2);
  
    	DMA0SA = (unsigned int) valuesX[i];
	}	
	
	
}
