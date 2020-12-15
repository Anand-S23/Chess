#ifndef CHESS_H 
#define CHESS_H

typedef enum turn
{
    TURN_white, 
    TURN_black
} turn;

typedef enum piece_type
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
} piece_type;

typedef struct piece
{
    int x; 
    int y; 
    piece_type type;
    b32 moved; 
} piece;

typedef struct selected
{
    piece piece; 
    b32 set;
} selected;

#include "undo.c"

typedef struct game_state
{
    piece board[BOARD_WIDTH][BOARD_HEIGHT];
    b32 board_initialized;
    stack moves; 
    turn current_turn; 
    piece current_selected;
    b32 selected_set
    SDL_Texture *textures[PIECE_max];
} game_state;

internal void ClearScreen(SDL_Renderer *renderer, u8 r, u8 g, u8 b, u8 a);
internal void UpdateApp(SDL_Renderer *renderer, game_state *state);

#endif