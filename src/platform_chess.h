#ifndef PLATFROM_CHESS_H
#define PLATFROM_CHESS_H 

#include <stdint.h> 

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef i32 b32;

#define internal static
#define global static
#define local_persist static

#define WINDOW_WIDTH    720
#define WINDOW_HEIGHT   720

#define BOARD_SIZE   64 
#define BOARD_WIDTH  8 
#define BOARD_HEIGHT 8 

#define CELL_WIDTH   (WINDOW_WIDTH / BOARD_WIDTH) 
#define CELL_HEIGHT  (WINDOW_HEIGHT / BOARD_HEIGHT)

typedef enum turn
{
    TURN_white, 
    TURN_black
} turn;

typedef enum pieces
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
} pieces;

typedef struct move
{
    pieces new_piece; 
    pieces old_piece; 
    int new_i; 
    int new_j; 
    int old_i; 
    int old_j; 
} move;

typedef struct selected
{
    pieces piece; 
    b32 set;
    int i; 
    int j; 
} selected;

typedef struct mouse_state
{
    int mouse_x; 
    int mouse_y; 
    b32 mouse_down;
    b32 mouse_up;
} mouse_state;

typedef struct game_state
{
    pieces board[BOARD_WIDTH][BOARD_HEIGHT];
    b32 board_initialized;
    turn current_turn; 
    mouse_state input;
    selected current_selected;
    SDL_Texture *textures[PIECE_max];
} game_state;

#endif
