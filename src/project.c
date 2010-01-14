#include "msp430x16x.h"        // Systemdefinitionen von TI für den MSP430F1612
#include "init.h"            // Initialisierung des Mikrocontrollers
#include "CC1100.h"            // CC1100 Funktransceiver
#include "system.h"            // Systemfunktionen MSB430H
#include "interrupts.h"        // ISR - Interrupt Service Routinen
#include "stdio.h"            // includes TI MSP430F1612 
#include "SHT11.h"            // SHT11 Temperatur- und Feuchtesensor
#include "project.h"
#include "math.h"

#define RED                    (0x01)
#define YELLOW                (0x02)
#define GREEN                (0x04)
#define LED_OFF(led)        (P4OUT |= led)    
#define LED_ON(led)          (P4OUT &= ~led)    
#define LED_TOGGLE(led)      (P4OUT ^=  led)

#define M_PI 3.1415926535897932384626433

// no lissajous anymore
//#define SCALEX 1
//#define SCALEY 1

#define FALSE 0
#define TRUE 1

#define FIELD_X 1024.0
#define FIELD_Y 1024.0
#define BARLENGTH 1024.0

#define RES_X 4095.0
#define RES_Y 4095.0

#define BALL_POINTS 10

int valuesX[ANTZ];
int valuesY[ANTZ];

int i;

volatile int ic=2;


void project(){
    
    ballstate ball;
    barstate left_bar=512, right_bar=512;

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
    TBCTL     = MC_1 + TASSEL_2 + ID_3;
    //TBCCTL0 = CCIE; 
    TBCCR0     = 3;
    
    _bis_SR_register(GIE); //Interrupts zulassen
    
    DAC12_1DAT=valuesY[0];
    DAC12_0DAT=valuesX[0];


// here be pong
    init_ball(&ball);
    
    while(1) {
        next(&ball, left_bar, right_bar);
        LED_TOGGLE(GREEN);
        generate_array(&ball, left_bar, right_bar);
    }    
}

uint8_t next(ballstate *ball, barstate bar_left, barstate bar_right)
{
        // move ball
        float m_x, m_y;
        
        // calulcate the movement vectors
        m_x = ball->speed * cos(ball->angle);
        m_y = ball->speed * sin(ball->angle);
        
        // add movement vectros to current position
        ball->x += m_x;
        ball->y += m_y;
        
        // check for collisions
        // ball has reached the borders
        if ((ball->y <= 0) || (ball->y >= FIELD_Y)) {
            ball->angle = 2*M_PI - ball->angle;
            
            if (ball->y >= FIELD_Y) {
                // ball crossed border at yMax
                ball->y = 2 * FIELD_Y - ball->y;
            }
            else {
                // ball crossed border at yMin
                ball->y = fabs(ball->y);
            }
            LED_TOGGLE(YELLOW);
        }
        
        // ball has passed bar layer
        if(ball->x <= 0 || ball->x >= 1024){

            float collision_point_y = fabs(tan(ball->angle) * (ball->x - m_x));
            float bar_position;
            
            if (ball->x <= 0) {
                // calculate if 
                bar_position = fabs(collision_point_y - (float)bar_left);
            }
            else {
                // calculate if 
                bar_position = fabs(collision_point_y - (float)bar_right);
            }

            if (ball->x >= FIELD_X) {
                // ball crossed border at xMax
                ball->x = 2 * FIELD_X - ball->x;
            }
            else {
                // ball crossed border at xMin
                ball->x = fabs(ball->x);
            }

            if (bar_position <= (BARLENGTH/2)) {
                ball->angle = (M_PI - ball->angle);
                //Use the speedup feature later when averything works
                //ball->speed += 0.2;
                LED_TOGGLE(RED);
                return TRUE;
            }
            
            return FALSE;
        }

        return TRUE;
}

void generate_array(ballstate *ball, barstate bar_left, barstate bar_right)
{
    int i, count;
    for(i=0;i<BALL_POINTS;i++){
        valuesX[i] = (int)ball->x * RES_X/FIELD_X;
        valuesY[i] = (int)ball->y * RES_Y/FIELD_Y;
    }
    
    count = ((ANTZ-BALL_POINTS) / 2);
    for (i = 1; i < count + 1; i++) {
        valuesX[i + BALL_POINTS-1] = 0;
        valuesY[i + BALL_POINTS-1] = (int)((bar_left - BARLENGTH/2.0) + (BARLENGTH / count) * i * RES_Y/FIELD_Y);
    }
    
    
    for (i = count + 1; i < ANTZ - BALL_POINTS; i++) {
        valuesX[i + BALL_POINTS-1] = (int)RES_X;
        valuesY[i + BALL_POINTS-1] = (int)((bar_right - BARLENGTH/2.0) + (BARLENGTH / count) * (i-count) * RES_Y/FIELD_Y);
    }
}




void init_ball(ballstate *ball)
{
    ball->x = FIELD_X / 2;
    ball->y = FIELD_Y / 2;
    ball->speed = 10;
    ball->angle = 0.4; // 7°
}

