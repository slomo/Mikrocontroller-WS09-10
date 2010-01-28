extern int ready;

#define RED                    (0x01)
#define YELLOW                (0x02)
#define GREEN                (0x04)
#define LED_OFF(led)        (P4OUT |= led)    
#define LED_ON(led)          (P4OUT &= ~led)    
#define LED_TOGGLE(led)      (P4OUT ^=  led)

void project();
