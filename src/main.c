#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WIDTH 800 /* width and height of window */
#define HEIGHT 600
#define RECT_DIM 100.0f /* dimensions of input blocks */

/* initialize window and renderer variables */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* ini background variables */
static SDL_Surface *bg_surface = NULL;
static SDL_Texture *bg_texture = NULL;

/* booleans for activated gates */
static bool active[4] = {false, false, false, false};
static bool gate_active[3] = {false, false, false};

/* all geometry and drawings (rendering) */
static SDL_FRect rects[4];
static SDL_FRect borders[4];
static SDL_FRect result, result_border;
static SDL_FRect wires[4];
static SDL_FRect gates[3];
static SDL_FRect gate_borders[3];
static SDL_FRect connect_wire, left_wire, right_wire;

/* operators and their gates */
static char operations[5][5] = {"AND", "OR", "NOR", "nAND", "XOR"};
static char gate_op[3][5];

// and or nand nor xor, no not because its not binary

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) /* runs once at startup, initialization */
{
    if (!SDL_CreateWindowAndRenderer("comp arch game", WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer)) /* creates window */
    {
        SDL_Log("could not create window and renderer: %s", SDL_GetError()); /* if fail, log error */
        return SDL_APP_FAILURE;
    }

    int x, y;
    SDL_GetRenderOutputSize(renderer, &x, &y);
    float y_pos = y / 4 - RECT_DIM / 2;

    for (int i = 0; i < SDL_arraysize(rects); i++) /* properties of blocks and borders initialized on app start*/
    {
        rects[i].w = rects[i].h = RECT_DIM;
        rects[i].x = 175.0f;
        rects[i].y = y_pos * (i + 1) - 50.0f;

        wires[i].w = 300.0f;
        wires[i].h = 10.0f;
        wires[i].x = rects[i].x + rects[i].w;
        wires[i].y = rects[i].y + 45.0f;

        borders[i].w = borders[i].h = RECT_DIM + 20.0f;
        borders[i].x = 165.0f;
        borders[i].y = rects[i].y - 10.0f;
    }

    result.x = ((float)x) - 100.0f;
    result.y = rects[0].y;
    result.w = 100.0f;
    result.h = rects[3].y - 75.0f;

    result_border.x = result.x - 10.0f;
    result_border.y = result.y - 10.0f;
    result_border.w = result.w + 20.0f;
    result_border.h = result.h + 20.0f;

    gates[0].x = wires[0].x + wires[0].w;
    gates[0].y = rects[0].y;
    gates[0].w = 150.0f;
    gates[0].h = 320.0f; 

    gate_borders[0].x = gates[0].x - 10.0f;
    gate_borders[0].y = gates[0].y - 10.0f;
    gate_borders[0].w = gates[0].w + 20.0f;
    gate_borders[0].h = gates[0].h + 20.0f;

    gates[1].x = wires[0].x + wires[0].w;
    gates[1].y = rects[2].y;
    gates[1].w = 150.0f;
    gates[1].h = 320.0f; 

    gate_borders[1].x = gates[1].x - 10.0f;
    gate_borders[1].y = gates[1].y - 10.0f;
    gate_borders[1].w = gates[1].w + 20.0f;
    gate_borders[1].h = gates[1].h + 20.0f;

    connect_wire.x = gates[0].x + 70.0f;
    connect_wire.y = gates[0].y + gates[0].h;
    connect_wire.w = wires[0].h;
    connect_wire.h = 150.0f;

    left_wire.x = connect_wire.x;
    left_wire.y = connect_wire.y + 50.0f;
    left_wire.w = 500.0f;
    left_wire.h = wires[0].h;

    gates[2].x = left_wire.x + left_wire.w;
    gates[2].y = left_wire.y - 160.0f;
    gates[2].w = 150.0f;
    gates[2].h = 320.0f;

    gate_borders[2].x = gates[2].x - 10.0f;
    gate_borders[2].y = gates[2].y - 10.0f;
    gate_borders[2].w = gates[2].w + 20.0f;
    gate_borders[2].h = gates[2].h + 20.0f;

    right_wire.x = gates[2].x + gates[2].w;
    right_wire.y = left_wire.y;
    right_wire.w = result.x - right_wire.x;
    right_wire.h = wires[0].h;

    for (int i = 0; i < SDL_arraysize(gate_op); i++)
    {
        strcpy(gate_op[i], operations[SDL_rand(5)]); /* randomly assigned gates */
    }

    return SDL_APP_CONTINUE; /* app continues if window created */
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_Q|| event->type == SDL_EVENT_QUIT) /* press 'q' to quit */
    {
        return SDL_APP_SUCCESS; /* end program reporting success */
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)  /* click to activate input gates */
    {
        for (int i = 0; i < SDL_arraysize(rects); i++)
        {
            if (event->button.x >= rects[i].x && event->button.x <= (rects[i].x + rects[i].w) &&
                event->button.y >= rects[i].y && event->button.y <= (rects[i].y + rects[i].h)) /* checking if clicked on gate rect */
            {
                active[i] = !active[i]; /* turning input gates on or off */
            }
        }
    }

    if (event->key.key == SDLK_R) /* press 'r' to reinitialize all values, effectively restarting */
    {
        for (int i = 0; i < SDL_arraysize(active); i++) active[i] = false;
        for (int i = 0; i < SDL_arraysize(gate_active); i++) gate_active[i] = false;
        for (int i = 0; i < SDL_arraysize(gate_op); i++) strcpy(gate_op[i], operations[SDL_rand(5)]);
    }
    

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    bg_surface = SDL_LoadBMP("assets/background.bmp");               /* load bitmap from assets to sdl surface */
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface); /* creates texture from sdl surface */
    SDL_RenderTexture(renderer, bg_texture, NULL, NULL);

    for (int i = 0; i < SDL_arraysize(borders); i++)                   /* if gate is active, color border green, else red */
    {
        if (active[i])
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &borders[i]);
    }

    if (gate_active[2]) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);   /* result block border coloring */
    else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &result_border);

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRects(renderer, wires, SDL_arraysize(wires));             /* wires connecting to blocks, mozda obojit i ovo jbg */
    
    for (int i = 0; i < SDL_arraysize(gate_active); i++)                    /* logic operations loop */
    {
        switch (gate_op[i][0])
        {
        case 'A':
            if ((i == 0 && active[0] && active[1]) || (i == 1 && active[2] && active[3]) || (i == 2 && gate_active[0] && gate_active[1]))
            {
                gate_active[i] = true;
            } else gate_active[i] = false;                                  /* checking if gates are true or false */
            break;
        case 'n':
            if ((i == 0 && !(active[0] && active[1])) || ((i == 1 && !(active[2] && active[3]))) || ((i == 2 && !(gate_active[0] && gate_active[1]))))
            {
                gate_active[i] = true;
            } else gate_active[i] = false;
            break;
        case 'O':
            if ((i == 0 && (active[0] || active[1])) || (i == 1 && (active[2] || active[3])) || (i == 2 && (gate_active[0] || gate_active[1])))
            {
                gate_active[i] = true;
            } else gate_active[i] = false;
            break;
        case 'N':
            if ((i == 0 && !(active[0] || active[1])) || (i == 1 && !(active[2] || active[3])) || (i == 2 && !(gate_active[0] || gate_active[1])))
            {
                gate_active[i] = true;
            } else gate_active[i] = false;
            break;
        case 'X':
            if ((i == 0 && (active[0] ^ active[1])) || (i == 1 && (active[2] ^ active[3])) || (i == 2 && (gate_active[0] ^ gate_active[1])))
            {
                gate_active[i] = true;
            } else gate_active[i] = false;
            break;
        default:
            break;
        }
    }
    
    for (int i = 0; i < SDL_arraysize(gate_active); i++)
    {
        if (gate_active[i]) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);       /* coloring gate borders, active or not */
        else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &gate_borders[i]);
    }
    
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRects(renderer, rects, SDL_arraysize(rects));                 /* rest of geometry, wires and rects */

    SDL_RenderFillRect(renderer, &result);
    SDL_RenderFillRects(renderer, gates, SDL_arraysize(gates));
    SDL_RenderFillRect(renderer, &connect_wire);
    SDL_RenderFillRect(renderer, &left_wire);
    SDL_RenderFillRect(renderer, &right_wire);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderScale(renderer, 2.0f, 2.0f);                       /* scaling because letters are too small */
    for (int i = 0; i < SDL_arraysize(gate_op); i++)
    {
        float x, y;
        x = (gates[i].x + gates[i].w / 2 - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(gate_op[i])) / 2;    
        y = (gates[i].y + gates[i].h / 2 - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;
        SDL_RenderDebugText(renderer, x, y, gate_op[i]);
    }
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);                       /* reseting the scale */

    SDL_RenderPresent(renderer); /* presents renderer to window */

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {} /* runs once on shutdown */