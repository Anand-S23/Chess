#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "platform_chess.h"
#include "stack.c"
#include "chess.c"

global b32 Global_Running;
global game_state Global_State = {0};

internal void HandleEvent(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_QUIT:
        {
            Global_Running = 0;
        } break;

        case SDL_MOUSEBUTTONDOWN:
        {
            Global_State.input.mouse_down = 
                (event->button.button == SDL_BUTTON_LEFT) ? 1 : 0; 
        } break;

        case SDL_MOUSEBUTTONUP:
        {
            Global_State.input.mouse_down = 
                (event->button.button == SDL_BUTTON_LEFT) ? 0 : 1; 
            Global_State.input.mouse_up = 
                (event->button.button == SDL_BUTTON_LEFT) ? 1 : 0; 
        } break;

        case SDL_MOUSEMOTION:
        {
            SDL_GetMouseState(&Global_State.input.mouse_x, 
                              &Global_State.input.mouse_y);
        } break;
    }
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s", SDL_GetError());
    }

    SDL_Window *window = SDL_CreateWindow("Chess",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 
                                                    SDL_RENDERER_ACCELERATED);

        if (renderer)
        {
            Global_Running = 1;
            while (Global_Running)
            {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    HandleEvent(&event);
                }

                UpdateApp(renderer, &Global_State);
            }
        }
        else
        {
            printf("SDL_CreateRenderer failed: %s", SDL_GetError());
        }
    }
    else
    {
        printf("SDL_CreateWindow failed: %s", SDL_GetError());
    }

    SDL_Quit();
    return 0;
}