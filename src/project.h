#define ANTZ 510
typedef struct ball {
        float x;     // x position of the ball
        float y;     // y position of the ball
        float angle;  // current absolute angle of movement
        float speed;  // current speed of the ball
} ballstate;

typedef float barstate;

extern int valuesX[ANTZ];
extern int valuesY[ANTZ];
extern volatile int ic;
extern int i;
void project();
uint8_t next(ballstate *ball, barstate bar_left, barstate bar_right);
void generate_array(ballstate *ball, barstate bar_left, barstate bar_right);
void init_ball(ballstate *ball);
