#ifndef CHESS_H 
#define CHESS_H

typedef enum turn
{
    TURN_white, 
    TURN_black
} turn;

typedef enum piece
{
    PIECE_none, 

    // white pieces
    PIECE_white_pawn, 
    PIECE_white_knight, 
    PIECE_white_bishop, 
    PIECE_white_rook, 
    PIECE_white_queen, 
    PIECE_white_king, 

    // black pieces
    PIECE_black_pawn, 
    PIECE_black_knight, 
    PIECE_black_bishop, 
    PIECE_black_rook, 
    PIECE_black_queen, 
    PIECE_black_king, 

    PIECE_max
} piece;

typedef struct selected
{
    piece type;
    int i; 
    int j; 
    b32 set;
} selected;

typedef struct move
{
    piece current_piece; 
    int current_i; 
    int current_j; 
    piece original_piece; 
    int original_i; 
    int original_j; 
} move;

#include "undo.h"

typedef struct game_state
{
    memory_arena permanent_arena;
    memory_arena transient_arena;

    piece board[BOARD_WIDTH][BOARD_HEIGHT];
    SDL_Texture *textures[PIECE_max];
    stack moves; 
    turn current_turn; 
    selected current_selected;
} game_state;

internal void ClearScreen(SDL_Renderer *renderer, u8 r, u8 g, u8 b, u8 a);
internal void UpdateApp(SDL_Renderer *renderer, platform *platform);

#endif