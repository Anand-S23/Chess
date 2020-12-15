#ifndef PLATFROM_CHESS_H
#define PLATFROM_CHESS_H 

#include <stdint.h> 
#include "stack.h"

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

typedef struct platform
{
    void *permanent_storage;
    u32   permanent_storage_size;
    void *transient_storage;
    u32   transient_storage_size;

    int mouse_x; 
    int mouse_y; 
    b32 mouse_down;
    b32 mouse_up;
    b32 mouse_right_down; 
    b32 mouse_right_up;
} platform; 

#endif
