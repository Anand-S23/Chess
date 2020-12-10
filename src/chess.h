#ifndef CHESS_H 
#define CHESS_H

internal void ClearScreen(SDL_Renderer *renderer, u8 r, u8 g, u8 b, u8 a);
internal void RendererBoard(SDL_Renderer *renderer);
internal void UpdateApp(SDL_Renderer *renderer, game_state *state);

#endif