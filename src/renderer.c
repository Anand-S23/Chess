#include "renderer.h"

internal void ClearScreen(SDL_Renderer *renderer, v4 color) 
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

internal void RenderFilledRect(SDL_Renderer *renderer, 
                               color color, v4 rect)
{
}

internal void RenderImage(SDL_Renderer *renderer, 
                          SDL_Texture *img_texture, v4 rect)
{
}