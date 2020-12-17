#include "chess.h"
#include "renderer.c"

internal void UpdateApp(SDL_Renderer *renderer, platform *platform)
{
    //Assert(sizeof(game_state) <= platform->permanent_storage_size);
    game_state *state = (game_state *)platform->permanent_storage;

    if (!platform->initialized)
    {
        state->permanent_arena = InitMemoryArena(platform->permanent_storage, 
                                                 platform->permanent_storage_size);
        state->transient_arena = InitMemoryArena(platform->transient_storage, 
                                                 platform->transient_storage_size);
        AllocateMemoryArena(&state->permanent_arena, sizeof(game_state));

        // Load piece textures
        for (int i = 1; i < PIECE_max; ++i)
        {
            switch (i)
            {
                case PIECE_white_pawn: 
                {
                    state->textures[PIECE_white_pawn] = 
                        IMG_LoadTexture(renderer, "data/wp.png");
                } break;

                case PIECE_white_knight: 
                {
                    state->textures[PIECE_white_knight] 
                        = IMG_LoadTexture(renderer, "data/wn.png");
                } break;
                
                case PIECE_white_bishop: 
                {
                    state->textures[PIECE_white_bishop] 
                        = IMG_LoadTexture(renderer, "data/wb.png");
                } break;

                case PIECE_white_rook: 
                {
                    state->textures[PIECE_white_rook] 
                        = IMG_LoadTexture(renderer, "data/wr.png");
                } break;

                case PIECE_white_queen: 
                {
                    state->textures[PIECE_white_queen] 
                        = IMG_LoadTexture(renderer, "data/wq.png");
                } break;

                case PIECE_white_king: 
                {
                    state->textures[PIECE_white_king] 
                        = IMG_LoadTexture(renderer, "data/wk.png");
                } break;

                case PIECE_black_pawn:
                {
                    state->textures[PIECE_black_pawn] 
                        = IMG_LoadTexture(renderer, "data/bp.png");
                } break;

                case PIECE_black_knight:
                {
                    state->textures[PIECE_black_knight] 
                        = IMG_LoadTexture(renderer, "data/bn.png");
                } break;

                case PIECE_black_bishop:
                {
                    state->textures[PIECE_black_bishop] 
                        = IMG_LoadTexture(renderer, "data/bb.png");
                } break;

                case PIECE_black_rook:
                {
                    state->textures[PIECE_black_rook] 
                        = IMG_LoadTexture(renderer, "data/br.png");
                } break;

                case PIECE_black_queen:
                {
                    state->textures[PIECE_black_queen] 
                        = IMG_LoadTexture(renderer, "data/bq.png");
                } break;

                case PIECE_black_king:
                {
                    state->textures[PIECE_black_king] 
                        = IMG_LoadTexture(renderer, "data/bk.png");
                } break;

                default: 
                {
                    printf("Error: %d not loaded", i);
                } break;
            }
        }

        // initalize board
        for (int j = 0; j < BOARD_HEIGHT; ++j)
        {
            for (int i = 0; i < BOARD_WIDTH; ++i)
            {
                piece current_piece; 

                switch (j)
                {
                    case 0: 
                    {
                        // black pieces
                        switch (i)
                        {
                            case 0:
                            case 7: 
                            {
                                current_piece = PIECE_black_rook;
                            } break; 

                            case 1:
                            case 6: 
                            {
                                current_piece = PIECE_black_knight;
                            } break; 

                            case 2:
                            case 5: 
                            {
                                current_piece = PIECE_black_bishop;
                            } break; 

                            case 3:
                            {
                                current_piece = PIECE_black_queen;
                            } break;

                            case 4: 
                            {
                                current_piece = PIECE_black_king;
                            } break; 
                        }
                    } break; 

                    case 1: 
                    {
                        current_piece = PIECE_black_pawn;
                    } break;

                    case 6: 
                    {
                        current_piece = PIECE_white_pawn;
                    } break;

                    case 7: 
                    {
                        // white pieces
                        switch (i)
                        {
                            case 0:
                            case 7: 
                            {
                                current_piece = PIECE_white_rook;
                            } break; 

                            case 1:
                            case 6: 
                            {
                                current_piece = PIECE_white_knight;
                            } break; 

                            case 2:
                            case 5: 
                            {
                                current_piece = PIECE_white_bishop;
                            } break; 

                            case 3:
                            {
                                current_piece = PIECE_white_queen;
                            } break;

                            case 4: 
                            {
                                current_piece = PIECE_white_king;
                            } break; 
                        }
                    } break;

                    default: 
                    {
                        current_piece = PIECE_none;
                    } break;
                }

                state->board[j][i] = current_piece;
            }
        }
         
        // TODO: look into using memory arena for the stack
        state->moves = CreateStack();

        state->current_turn = TURN_white; 

        platform->initialized = 1;
    }

    if (platform->mouse_down)
    {
        int i = (platform->mouse_x - platform->mouse_x % CELL_WIDTH) / CELL_WIDTH; 
        int j = (platform->mouse_y - platform->mouse_y % CELL_HEIGHT) / CELL_HEIGHT; 

        piece current_piece = state->board[j][i];
        if (((state->current_turn == TURN_white && (current_piece <= PIECE_white_king && current_piece >= PIECE_white_pawn)) ||
             (state->current_turn == TURN_black && (current_piece <= PIECE_black_king && current_piece >= PIECE_black_pawn))) && 
            !state->current_selected.set)
        {
            state->current_selected.set = 1;
            state->current_selected.type = current_piece;
            state->current_selected.i = i; 
            state->current_selected.j = j;
            state->board[j][i] = PIECE_none;
        }
    }

    if (platform->mouse_up)
    {
        if (state->current_selected.type != PIECE_none)
        {
            int i = (platform->mouse_x - platform->mouse_x % CELL_WIDTH) / CELL_WIDTH; 
            int j = (platform->mouse_y - platform->mouse_y % CELL_HEIGHT) / CELL_HEIGHT; 

            if ((i != state->current_selected.i) || (j != state->current_selected.j))
            {
                move new_move = {0};
                {
                    new_move.current_piece = state->current_selected.type;
                    new_move.current_i = i;
                    new_move.current_j = j;
                    new_move.original_piece = state->board[j][i];
                    new_move.original_i = state->current_selected.i;
                    new_move.original_j = state->current_selected.j;
                }

                state->board[j][i] = state->current_selected.type;
                Push(state->moves, new_move);
                state->current_turn = (state->current_turn == TURN_white) ? TURN_black : TURN_white;
            }
            else
            {
                int old_j = state->current_selected.j;
                int old_i = state->current_selected.i;
                state->board[old_j][old_i] = state->current_selected.type;
            }
        }

        state->current_selected.type = PIECE_none;
        state->current_selected.set = 0;
        platform->mouse_up = 0;
    }

    if (platform->mouse_right_up)
    {
        if (!Empty(state->moves))
        {
            move undo = Pop(state->moves);
            state->board[undo.original_j][undo.original_i] = undo.current_piece;
            state->board[undo.current_j][undo.current_i]   = undo.original_piece;
        }
    }

    ClearScreen(renderer, v4(0, 0, 0, 255));

    for (int j = 0; j < BOARD_HEIGHT; ++j)
    {
        for (int i = 0; i < BOARD_WIDTH; ++i)
        {
            if (i % 2 == j % 2)
            {
                RenderFilledRect(renderer, v4(124, 76, 64, 255), 
                                 v4(i * CELL_WIDTH, j * CELL_HEIGHT,
                                    CELL_WIDTH, CELL_HEIGHT));
            }
            else 
            {
                RenderFilledRect(renderer, v4(81, 42, 42, 255), 
                                 v4(i * CELL_WIDTH, j * CELL_HEIGHT,
                                    CELL_WIDTH, CELL_HEIGHT));
            }

            if (state->board[j][i] != PIECE_none)
            {
                RenderImage(renderer, state->textures[state->board[j][i]], 
                            v4(i * CELL_WIDTH, j * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT));
            }
        }
    }

    if (state->current_selected.type != PIECE_none)
    {
        RenderImage(renderer, state->textures[state->current_selected.type], 
                    v4(platform->mouse_x - CELL_WIDTH / 2, 
                       platform->mouse_y - CELL_HEIGHT / 2,
                       CELL_WIDTH, CELL_HEIGHT));
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}