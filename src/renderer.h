#ifndef RENDERER_H
#define RENDERER_H

internal void ClearScreen(SDL_Renderer *renderer, v4 color);
internal void RenderFilledRect(SDL_Renderer *renderer, v4 color, v4 rect);
internal void RenderImage(SDL_Renderer *renderer, SDL_Texture *img_texture, v4 rect);

#endif