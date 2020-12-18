#ifndef CHESS_H 
#define CHESS_H

typedef enum mode
{
    MODE_menu,
    MODE_freeplay
} mode;

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

    PIECE_white,

    // black pieces
    PIECE_black_pawn, 
    PIECE_black_knight, 
    PIECE_black_bishop, 
    PIECE_black_rook, 
    PIECE_black_queen, 
    PIECE_black_king, 

    PIECE_black,

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
    piece moved_piece; 
    int original_i; 
    int original_j; 
    piece taken_piece; 
    int moved_to_i; 
    int moved_to_j; 
} move;

typedef struct castle
{
    b32 white_can_castle_left;
    b32 white_can_castle_right;
    b32 black_can_castle_left;
    b32 black_can_castle_right;
} castle;

#include "undo.h"

typedef struct game_state
{
    memory_arena permanent_arena;
    memory_arena transient_arena;

    piece board[BOARD_WIDTH][BOARD_HEIGHT];
    SDL_Texture *textures[PIECE_max];
    SDL_Texture *text[2];
    stack moves; 
    turn current_turn; 
    mode current_mode;
    selected current_selected;
    castle castle;
} game_state;

internal void UpdateMenu(SDL_Renderer *renderer, platform *platform, game_state *state);
internal void UpdateApp(SDL_Renderer *renderer, platform *platform);

#endif