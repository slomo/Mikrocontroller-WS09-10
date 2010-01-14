//      testscope.c
//      
//      Copyright 2010 Robin Nehls <nehls@mi.fu-berlin.de>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include <signal.h>

#define WINDOW_X 512
#define WINDOW_Y 512

//********************************************************************//
#include <math.h>

#define FALSE 0
#define TRUE 1

#define FIELD_X 1024
#define FIELD_Y 1024
#define BARLENGTH 1024
#define ANTZ 210
#define BALL_POINTS 10

#define RES_X 500
#define RES_Y 500

typedef struct ball {
        float x;     // x position of the ball
        float y;     // y position of the ball
        float angle;  // current absolute angle of movement
        float speed;  // current speed of the ball
} ballstate;

typedef float barstate;

int valuesX[ANTZ];
int valuesY[ANTZ];

void init_ball(ballstate *ball)
{
    ball->x = FIELD_X / 2;
    ball->y = FIELD_Y / 2;
    ball->speed = 1;
    ball->angle = 0.122173; // 7°
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
            //LED_TOGGLE(YELLOW);
        }
        
        // ball has passed bar layer
        if(ball->x <= 0 || ball->x >= 1024){

            float collision_point_y = fabs(tan(ball->angle) * (ball->x - m_x));
            float bar_position;
            
            if (ball->x <= 0) {
                // calculate if 
                bar_position = fabs(collision_point_y - bar_left);
            }
            else {
                // calculate if 
                bar_position = fabs(collision_point_y - bar_right);
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
                ball->speed += 0.01;
                //LED_TOGGLE(RED);
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
        valuesX[i + BALL_POINTS] = 0;
        valuesY[i + BALL_POINTS] = (int)((bar_left - BARLENGTH/2.0) + (BARLENGTH / count) * i * RES_Y/FIELD_Y);
    }
    
    
    for (i = count + 1; i < ANTZ-BALL_POINTS; i++) {
        valuesX[i + BALL_POINTS] = (int)RES_X;
        valuesY[i + BALL_POINTS] = (int)((bar_right - BARLENGTH/2.0) + (BARLENGTH / count) * (i-count) * RES_Y/FIELD_Y);
    }
}
//********************************************************************//





volatile int stop = 0;
void signalCallback(int i) {
    stop=1;
}

// function that draws a single pixel to a surface
void DrawPixel(SDL_Surface *Surface, int x, int y, Uint16 color)
{
    Uint16 *Pixel;
    if((x>=0) && (x<=RES_X) && (y>=0) && (y<=RES_Y)) {
        Pixel = (Uint16 *)Surface->pixels + y*Surface->pitch/2 + x*2;
        *Pixel = color;
    }
}

void DrawBigPixel(SDL_Surface *Surface, int x, int y,Uint16 color) {
    DrawPixel(Surface, x-1, y-1, color);
    DrawPixel(Surface, x, y-1, color);
    DrawPixel(Surface, x+1, y-1, color);
    
    DrawPixel(Surface, x-1, y, color);
    DrawPixel(Surface, x, y, color);
    DrawPixel(Surface, x+1, y, color);
    
    DrawPixel(Surface, x-1, y+1, color);
    DrawPixel(Surface, x, y+1, color);
    DrawPixel(Surface, x+1, y+1, color);
}

int main(int argc, char** argv)
{
	signal(SIGINT, signalCallback);

	// color for the pixels
	Uint32 color;
	
	// surface that will hold the background and the gfx
	SDL_Surface *screen = NULL;
	SDL_Surface *pixels = NULL;

	// initialize SDL video subsystem
	if(  SDL_Init(SDL_INIT_VIDEO) == -1 ) {
		return EXIT_FAILURE;
	}

	// Set up the screen with WINDOW_X*WINDOW_Y px size, 32 bit color and a
	// softwaresurface
	screen = SDL_SetVideoMode(WINDOW_X, WINDOW_Y, 32, SDL_SWSURFACE);
	if( screen == NULL ) {
		return EXIT_FAILURE;
	}

	// set color for pixels to green
	color = SDL_MapRGB(screen->format,0x00,0xFF,0x00);

	// Set the window caption
	SDL_WM_SetCaption("Testscope v0.1", NULL);

// ball init
ballstate myBall;
init_ball(&myBall);

while(!stop) {
	// create a new surface for the pixels
	pixels = SDL_CreateRGBSurface(SDL_SWSURFACE,WINDOW_X,WINDOW_Y,32,0,0,0,0);

	//****************************************************************//
	//Draw your stuff here
	int i;
	next(&myBall,512,512);
	generate_array(&myBall,512,512);
	for(i=0;i<ANTZ;i++) {
		DrawBigPixel(pixels, valuesX[i], valuesY[i], color);
		DrawBigPixel(pixels, valuesX[i]+1, valuesY[i], color);
		DrawBigPixel(pixels, valuesX[i], valuesY[i]+1, color);
		DrawBigPixel(pixels, valuesX[i]-1, valuesY[i], color);
		DrawBigPixel(pixels, valuesX[i], valuesY[i]-1, color);
	}
	//****************************************************************//

	SDL_Rect rect;
	rect.x = 6;
	rect.y = 6;
	rect.w = 500;
	rect.h = 500;
	// Apply image to screen
	SDL_BlitSurface(pixels, NULL, screen, &rect);
	// Update Screen
	SDL_Flip(screen);
	// kill the surface
	SDL_FreeSurface(pixels);
}

	// Quit SDL
	SDL_Quit();

	printf("Bye.\n");

	return EXIT_SUCCESS;
}
