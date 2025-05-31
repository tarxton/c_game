#define SDL_MAIN_USE_CALLBACKS 1                                                /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WIDTH 800                                                               /* width and height of window */
#define HEIGHT 600
#define RECT_DIM 100.0f                                                         /* dimensions of input blocks */

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Surface *bg_surface = NULL;
static SDL_Texture *bg_texture = NULL;
static SDL_FRect rects[4];
static SDL_FRect borders[4];

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

    for (int i = 0; i < SDL_arraysize(rects); i++)                              /* properties of blocks and borders initialized on app start*/
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
    bg_surface = SDL_LoadBMP("assets/background.bmp");                            /* load bitmap from assets to sdl surface */
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);              /* creates texture from sdl surface */
    SDL_RenderTexture(renderer, bg_texture, NULL, NULL);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                             /* draw input blocks and borders */
    SDL_RenderFillRects(renderer, borders, SDL_arraysize(borders));
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRects(renderer, rects, SDL_arraysize(rects));

    SDL_RenderPresent(renderer);                                                /* presents renderer to window */

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}                       /* runs once on shutdown */