#define SDL_MAIN_USE_CALLBACKS 1                                                /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WIDTH 800                                                               /* width and height of window */
#define HEIGHT 600
#define RECT_DIM 100.0f

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Surface *bg_surface = NULL;
static SDL_Texture *bg_texture = NULL;
static SDL_FRect rects[4];
static SDL_FRect borders[4];

/*
struct Circle
{
    int x;
    int y;
    SDL_FColor color;
};
*/


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])              /* runs once at startup, initialization */
{
    if (!SDL_CreateWindowAndRenderer("comp arch game", WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer))     /* creates window */
    {
        SDL_Log("could not create window and renderer: %s", SDL_GetError());    /* if fail, log error */
        return SDL_APP_FAILURE;                                              
    }

    int x,y;
    SDL_GetRenderOutputSize(renderer, &x, &y);
    float y_pos = y/4 - RECT_DIM/2;

    for (int i = 0; i < SDL_arraysize(rects); i++)
    {
        rects[i].w = rects[i].h = RECT_DIM;
        rects[i].x = 175.0f;
        rects[i].y = y_pos * (i+1) - 50.0f;

        borders[i].w = borders[i].h = RECT_DIM + 20.0f;
        borders[i].x = 165.0f;
        borders[i].y = rects[i].y - 10.0f;
    }
    

    return SDL_APP_CONTINUE;                                                    /* app continues if window created */
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) 
{
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT)     /* quit on key down */
    {
        return SDL_APP_SUCCESS;                                                 /* end program reporting success */
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    const char *msg = "ARHITEKTURA RAHHHHHHHH";
    int w = 0, h = 0;                                                           /* window resolution */
    float x, y;                                                                 /* top left coordinates of message */
    const float scale = 4.0f;                                                   /* scale to text */

    SDL_GetRenderOutputSize(renderer, &w, &h);                                    /* gets size of render target (window) in pixels */
    //SDL_SetRenderScale(renderer, scale, scale);                                   /* scale all rendering, drawing at (x,y) now draws at (x*scale, y*scale) */
    //x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(msg)) / 2; /* horizontal center */
    //y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;                   /* vertical center */

    bg_surface = SDL_LoadBMP("assets/background.bmp");
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);

    //SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);                            /* sets color */
    //SDL_RenderClear(renderer);                                                  /* clears screen with current color */
    
    SDL_RenderTexture(renderer, bg_texture, NULL, NULL);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRects(renderer, borders, SDL_arraysize(borders));
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRects(renderer, rects, SDL_arraysize(rects));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);                           
    //SDL_RenderDebugText(renderer, x, y, msg);                                   /* draws text */
    SDL_RenderPresent(renderer);                                                /* presents renderer to window */



    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}                       /* runs once on shutdown */