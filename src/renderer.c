#include "renderer.h"

internal void ClearScreen(SDL_Renderer *renderer, v4 color) 
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

internal void RenderFilledRect(SDL_Renderer *renderer, 
                               v4 color, v4 rect)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect render_rect = { rect.x, rect.y, rect.width, rect.height };
    SDL_RenderFillRect(renderer, &render_rect);
}

internal void RenderImage(SDL_Renderer *renderer, 
                          SDL_Texture *img_texture, v4 rect)
{
    int w, h;
    SDL_QueryTexture(img_texture, NULL, NULL, &w, &h);
    SDL_Rect render_rect = { rect.x + 5, rect.y + 5,
                             rect.width - 10, rect.height - 10 }; 
    SDL_RenderCopy(renderer, img_texture, NULL, &render_rect);
}