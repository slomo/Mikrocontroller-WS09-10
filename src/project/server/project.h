#define IN 0
#define LEFT -1
#define RIGHT 1

#define RED                    (0x01)
#define YELLOW                (0x02)
#define GREEN                (0x04)
#define LED_OFF(led)        (P4OUT |= led)    
#define LED_ON(led)          (P4OUT &= ~led)    
#define LED_TOGGLE(led)      (P4OUT ^=  led)

#define M_PI 3.1415926535897932384626433

#define RES_X 800.0
#define RES_Y 512.0
#define FIELD_X 1024.0
#define FIELD_Y 1024.0

#define BARLENGTH 300.0
#define BARSTEP 50.0
#define BALL_POINTS 4
#define ANTZ 50

typedef struct ball {
        float x;     // x position of the ball
        float y;     // y position of the ball
        float angle;  // current absolute angle of movement
        float speed;  // current speed of the ball
} ballstate;

typedef float barstate;

extern barstate left_bar;
extern barstate right_bar;

extern int valuesX[ANTZ];
extern int valuesY[ANTZ];

extern int life_left;
extern int life_right;

extern int running;

extern ballstate ball;

extern volatile int ic;
extern int i;

extern char str[255]; //man braucht immer mal str

extern int player;
extern int start_game;

void project();
int next(ballstate *ball, barstate bar_left, barstate bar_right);
void generate_array(ballstate *ball, barstate bar_left, barstate bar_right);
void init(ballstate *ball);
void reset_ball(ballstate *ball, int direction);

float normalize_radiant(float source);

void start_game_now();
