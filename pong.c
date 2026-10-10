#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include <stdio.h>
#include <stdlib.h>
// #include <math.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

static int MOVEMENT_SPEED = 10;
static int BALL_MOVEMENT_SPEED = 10;
static int LEFT_INNER_BORDER = 100;
static int RIGHT_INNER_BORDER = 980;
static int PADDLE_HEIGHT = 100;
const char* FONT_PATH = "font/PressStart2P-Regular.ttf";

static int COLOR = 0xffffffff;

typedef struct Speed {
   int x, y; 
} Speed;

typedef struct Score {
   int paddle1, paddle2; 
} Score;

void move_rect(SDL_Surface* surface, SDL_Rect* rect, Speed* speed) {
    SDL_FillSurfaceRect(surface, rect, 0x00000000);
    rect->x += speed->x;
    rect->y += speed->y;
    SDL_FillSurfaceRect(surface, rect, COLOR);
}

void move_paddle(SDL_Surface* surface, SDL_Rect* paddle, int direction) {
    if (direction < 0 && paddle->y <= 0)
        return;
    if (direction > 0 && paddle->y >= surface->h - paddle->h)
        return;    
    Speed speed = (Speed) {0, direction * MOVEMENT_SPEED};
    move_rect(surface, paddle, &speed);
}

void move_ball(
        SDL_Surface* surface, 
        SDL_Rect* ball, 
        SDL_Rect* paddle1, 
        SDL_Rect* paddle2, 
        Speed* ball_speed,
        Score* score
) {
   if (ball->x <= LEFT_INNER_BORDER) {
       if ((ball->y + ball->h > paddle1->y) && (ball->y < paddle1->y + paddle1->h)) {
           ball_speed->x = -ball_speed->x;
           double hit_frac = (
                   (ball->y + ball->h / (double)2) - (paddle1->y + paddle1->h / (double)2)
           ) / (PADDLE_HEIGHT / 2.0);     
           ball_speed->y = hit_frac * BALL_MOVEMENT_SPEED;
       } else {
           score->paddle2++;
           return;
       }
   }
   if (ball->x + ball->w >= RIGHT_INNER_BORDER) {
       if ((ball->y + ball->h > paddle2->y) && (ball->y < paddle2->y + paddle2->h)) {
           ball_speed->x = -ball_speed->x;
           double hit_frac = (
                   (ball->y + ball->h / (double)2) - (paddle2->y + paddle2->h / (double)2)
           ) / (PADDLE_HEIGHT / 2.0);
           ball_speed->y = hit_frac * BALL_MOVEMENT_SPEED;
       } else {
           score->paddle1++;
           return;
       }
   }

   if (ball->y <= 0)
       ball_speed->y = -ball_speed->y;
   if (ball->y + ball->h >= surface->h)
       ball_speed->y = -ball_speed->y;

   move_rect(surface, ball, ball_speed); 
}

void draw_border(SDL_Surface* surface) {
    for (int i = 0; i < surface->h; i += 20) {
        SDL_Rect border_block = (SDL_Rect) {surface->w / 2 - 2, i, 2, 10};
        SDL_FillSurfaceRect(surface, &border_block, COLOR);    
    }
}

void renderScores(SDL_Surface* surface, Score* score, TTF_Font* font) {
    SDL_Color color = {255, 255, 255, 255};

    int s1 = score->paddle1, s2 = score->paddle2;
    char str1[16], str2[16];
    snprintf(str1, sizeof(str1), "%d", s1);
    snprintf(str2, sizeof(str2), "%d", s2);

    SDL_Surface* text_score1 = TTF_RenderText_Solid(font, str1, 0, color);
    SDL_Surface* text_score2 = TTF_RenderText_Solid(font, str2, 0, color);

    if (!text_score1 || !text_score2) {
        SDL_Log("Failed to render score text: %s", SDL_GetError());

        if (text_score1) SDL_DestroySurface(text_score1);
        if (text_score2) SDL_DestroySurface(text_score2);
        return;
    }

    SDL_Rect dst_score1 = {surface->w / 2 - 110, 50, 0, 0};
    SDL_Rect dst_score2 = {surface->w / 2 + 20, 50, 0, 0};
    SDL_BlitSurface(text_score1, NULL, surface, &dst_score1);
    SDL_BlitSurface(text_score2, NULL, surface, &dst_score2);

    SDL_DestroySurface(text_score1);
    SDL_DestroySurface(text_score2);    
}

void renderWinningText(SDL_Surface* surface, TTF_Font* font, const char* text) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, 0, color);
    if (!text_surface) {
        SDL_Log("Failed to render score text: %s", SDL_GetError());
        SDL_DestroySurface(text_surface);
        return;
    }
    SDL_Rect dst_rect = {
        (surface->w - text_surface->w) / 2,
        (surface->h - text_surface->h) / 2,
        text_surface->w,
        text_surface->h
    };
    SDL_BlitSurface(text_surface, NULL, surface, &dst_rect);
    SDL_DestroySurface(text_surface);    
}

void initialize_game(
        SDL_Surface* surface, 
        SDL_Rect* paddle1, 
        SDL_Rect* paddle2, 
        SDL_Rect* ball,
        Speed* ball_speed
) {
    SDL_Rect screen = (SDL_Rect) {0, 0, surface->w, surface->h};
    SDL_FillSurfaceRect(surface, &screen, 0x00000000);
    
    // Draw the borders  
    draw_border(surface);

    // Draw left paddle
    *paddle1 = (SDL_Rect) {80, 310, 20, PADDLE_HEIGHT};
    SDL_FillSurfaceRect(surface, paddle1, COLOR);
  
    // Draw right paddle 
    *paddle2 = (SDL_Rect) {RIGHT_INNER_BORDER, 310, 20, PADDLE_HEIGHT};
    SDL_FillSurfaceRect(surface, paddle2, COLOR);

    // Draw the ball
    *ball = (SDL_Rect) {surface->w / 2 - 50, surface->h / 2, 10, 10};
    SDL_FillSurfaceRect(surface, ball, COLOR);
    *ball_speed = (Speed) {BALL_MOVEMENT_SPEED, 0};
}

int main() {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont(FONT_PATH, 100);
    TTF_Font* win_font = TTF_OpenFont(FONT_PATH, 60);

    SDL_Window* window = SDL_CreateWindow("Pong 1972", 1080, 720, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    
    SDL_Rect paddle1, paddle2, ball;
    Speed ball_speed;

    Score score = {0, 0};

    initialize_game(surface, &paddle1, &paddle2, &ball, &ball_speed);    

    // Update the window surface with the renders
    SDL_UpdateWindowSurface(window);
   
    int local_paddle1_score = 0, local_paddle2_score = 0;
    int running = 1;
    SDL_Event event;

    const bool* key_states = SDL_GetKeyboardState(NULL); 

    while (running) {
        if (local_paddle1_score == 5) {
            renderWinningText(surface, win_font, "PLAYER 1 WINS!");
            SDL_UpdateWindowSurface(window);
            SDL_Delay(5000);
            break;
        } else if (local_paddle2_score == 5) {
            renderWinningText(surface, win_font, "PLAYER 2 WINS!");
            SDL_UpdateWindowSurface(window);
            SDL_Delay(5000);
            break;
        }
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            running = 0;
        }

        if (key_states[SDL_SCANCODE_W]) {
            move_paddle(surface, &paddle1, -1);
        }
        if (key_states[SDL_SCANCODE_S]) {
            move_paddle(surface, &paddle1, 1); 
        }
        if (key_states[SDL_SCANCODE_UP]) { 
            move_paddle(surface, &paddle2, -1);
        }
        if (key_states[SDL_SCANCODE_DOWN]) { 
            move_paddle(surface, &paddle2, 1); 
        }

        // Move ball
        move_ball(surface, &ball, &paddle1, &paddle2, &ball_speed, &score);

        if (score.paddle1 != local_paddle1_score) {
            local_paddle1_score = score.paddle1;
            initialize_game(surface, &paddle1, &paddle2, &ball, &ball_speed); 
        }
        if (score.paddle2 != local_paddle2_score) {
            local_paddle2_score = score.paddle2;
            initialize_game(surface, &paddle1, &paddle2, &ball, &ball_speed); 
        }

        renderScores(surface, &score, font);
        // Redraw border everytime the ball crosses the border
        draw_border(surface);
         
        SDL_UpdateWindowSurface(window); 
        SDL_Delay(10); 
    }
    TTF_CloseFont(font);  
    TTF_Quit();
    return 0;
}
