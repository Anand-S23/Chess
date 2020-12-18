#include "chess.h"
#include "renderer.c"
#include "ui.c"

internal void UndoMove(game_state *state, move undo)
{
    state->board[undo.original_j][undo.original_i] = undo.moved_piece;
    state->board[undo.moved_to_j][undo.moved_to_i] = undo.taken_piece;
    state->current_turn = (state->current_turn == TURN_white) ? TURN_black : TURN_white;
}

internal b32 OppositeColorPiece(move move)
{
    b32 result = 1;

    if (move.moved_piece < PIECE_white && 
        move.taken_piece < PIECE_white &&
        move.taken_piece > PIECE_none)
    {
        result = 0;
    }
    else if (move.moved_piece > PIECE_white && 
             move.taken_piece > PIECE_white && 
             move.taken_piece < PIECE_max)
    {
        result = 0;
    }

    return result;
}

internal b32 CheckDiagonal(game_state *state, move move)
{
    int result = 0;

    int diff_i = move.moved_to_i - move.original_i;
    int diff_j = move.moved_to_j - move.original_j;

    if (OppositeColorPiece(move))
    {
        if (AbsoluteValue(diff_i) == AbsoluteValue(diff_j))
        {
            int i_inc = (diff_i > 0) ? -1 : 1;
            int j_inc = (diff_j > 0) ? -1 : 1;

            int current_i = move.moved_to_i + i_inc;
            int current_j = move.moved_to_j + j_inc;
            result = 1;
            while (current_i != move.original_i && current_j != move.original_j)
            {
                if (state->board[current_j][current_i] != PIECE_none)
                {
                    result = 0;
                    break;
                }

                current_i += i_inc; 
                current_j += j_inc;
            }
        }
    }

    return result;
}

internal b32 CheckLine(game_state *state, move move)
{
    int result = 0;

    int diff_i = move.moved_to_i - move.original_i;
    int diff_j = move.moved_to_j - move.original_j;

    if (OppositeColorPiece(move))
    {
        if ((AbsoluteValue(diff_i) == 0 && AbsoluteValue(diff_j) != 0) ||
            (AbsoluteValue(diff_i) != 0 && AbsoluteValue(diff_j) == 0))
        {
            int i_inc = (diff_i > 0) ? -1 : 1;
            i_inc = (diff_i == 0) ? 0 : i_inc;
            int j_inc = (diff_j > 0) ? -1 : 1;
            j_inc = (diff_j == 0) ? 0 : j_inc;


            result = 1;
            int current_i = move.moved_to_i + i_inc;
            int current_j = move.moved_to_j + j_inc;
            while (current_i != move.original_i || current_j != move.original_j)
            {
                if (state->board[current_j][current_i] != PIECE_none)
                {
                    result = 0;
                    break;
                }

                current_i += i_inc; 
                current_j += j_inc;
            }
        }
    }

    return result;
}

internal b32 CheckMoveValidity(game_state *state, move move)
{
    b32 result = 0;

    switch (move.moved_piece)
    {
        case PIECE_white_pawn: 
        {
            if (move.original_j == 6 && 
                move.moved_to_j == 4 &&
                move.original_i == move.moved_to_i &&
                move.taken_piece == PIECE_none && 
                state->board[move.original_j - 1][move.original_i] == PIECE_none)
            {
                result = 1;
            }
            else if (move.moved_to_j == (move.original_j - 1))
            {
                if (move.moved_to_i == (move.original_i - 1) ||
                    move.moved_to_i == (move.original_i + 1))
                {
                    if (OppositeColorPiece(move) && move.taken_piece != PIECE_none)
                    {
                        result = 1;
                    }
                }
                else if (move.moved_to_i == move.original_i &&
                         move.taken_piece == PIECE_none)
                {
                    result = 1;
                }
            }

            if (move.moved_to_j == 0)
            {
                state->board[move.moved_to_j][move.moved_to_i] = PIECE_white_queen;
            }
        } break;

        case PIECE_black_pawn: 
        {
            if (move.original_j == 1 && 
                move.moved_to_j == 3 &&
                move.original_i == move.moved_to_i &&
                move.taken_piece == PIECE_none && 
                state->board[move.original_j + 1][move.original_i] == PIECE_none)
            {
                result = 1;
            }
            else if (move.moved_to_j == (move.original_j + 1))
            {
                if (move.moved_to_i == (move.original_i - 1) ||
                    move.moved_to_i == (move.original_i + 1))
                {
                    if (OppositeColorPiece(move) && move.taken_piece != PIECE_none)
                    {
                        result = 1;
                    }
                }
                else if (move.moved_to_i == move.original_i &&
                         move.taken_piece == PIECE_none)
                {
                    result = 1;
                }
            }

            if (move.moved_to_j == 7)
            {
                state->board[move.moved_to_j][move.moved_to_i] = PIECE_black_queen;
            }
        } break;

        case PIECE_white_bishop: 
        case PIECE_black_bishop: 
        {
            result = CheckDiagonal(state, move);
        } break;

        case PIECE_white_knight: 
        case PIECE_black_knight: 
        {
            int diff_i = AbsoluteValue(move.moved_to_i - move.original_i);
            int diff_j = AbsoluteValue(move.moved_to_j - move.original_j);

            if (OppositeColorPiece(move))
            {
                if ((diff_j == 2 && diff_i == 1) ||
                    (diff_j == 1 && diff_i == 2))
                {
                    result = 1;
                }
            }
        } break;

        case PIECE_white_rook: 
        case PIECE_black_rook: 
        {
            result = CheckLine(state, move);

            if (move.original_i == 7 && move.original_j == 7)
            {
                state->castle.white_can_castle_left = 0;
            }
            else if (move.original_i == 7 && move.original_j == 0)
            {
                state->castle.black_can_castle_left = 0;
            }
            else if (move.original_i == 0 && move.original_j == 7)
            {
                state->castle.black_can_castle_left = 0;
            }
            else if (move.original_i == 0 && move.original_j == 0)
            {
                state->castle.black_can_castle_right = 0;
            }
        } break;

        case PIECE_white_queen: 
        case PIECE_black_queen: 
        {
            result = (CheckDiagonal(state, move) || CheckLine(state, move));
        } break;

        case PIECE_white_king: 
        case PIECE_black_king: 
        {
            int diff_i = move.moved_to_i - move.original_i;
            int diff_j = move.moved_to_j - move.original_j;

            if (OppositeColorPiece(move))
            {
                if (diff_j == 0 && diff_i == 2)
                {
                    if (CheckLine(state, move))
                    {
                        if (move.moved_piece == PIECE_white_king &&
                            state->castle.white_can_castle_right)
                        {
                            result = 1; 
                            state->castle.white_can_castle_right = 0;
                            state->board[7][5] = PIECE_white_rook;
                            state->board[7][7] = PIECE_none;
                        }
                        else if (move.moved_piece == PIECE_black_king &&
                                 state->castle.black_can_castle_right)
                        {
                            result = 1; 
                            state->castle.black_can_castle_right = 0;
                            state->board[0][5] = PIECE_black_rook;
                            state->board[0][7] = PIECE_none;
                        }
                    }
                }
                else if (diff_j == 0 && diff_i == -2)
                {
                    if (CheckLine(state, move))
                    {
                        if (move.moved_piece == PIECE_white_king &&
                            state->castle.white_can_castle_left)
                        {
                            result = 1; 
                            state->castle.white_can_castle_left = 0;
                            state->board[7][3] = PIECE_white_rook;
                            state->board[7][0] = PIECE_none;
                        }
                        else if (move.moved_piece == PIECE_black_king &&
                                 state->castle.black_can_castle_left)
                        {
                            result = 1; 
                            state->castle.black_can_castle_left = 0;
                            state->board[0][3] = PIECE_black_rook;
                            state->board[0][0] = PIECE_none;
                        }
                    }
                }
                if ((AbsoluteValue(diff_i) == 1 || AbsoluteValue(diff_i) == 0) &&
                    (AbsoluteValue(diff_j) == 0 || AbsoluteValue(diff_j) == 1))
                {
                    if (move.moved_piece == PIECE_white_king)
                    {
                        state->castle.white_can_castle_left = 0;
                        state->castle.white_can_castle_right = 0;
                    }
                    else if (move.moved_piece == PIECE_black_king)
                    {
                        state->castle.black_can_castle_left = 0;
                        state->castle.black_can_castle_right = 0;
                    }

                    result = 1;
                }
            }
        } break;
    }

    return result;
}

internal void UpdateMenu(SDL_Renderer *renderer, platform *platform, game_state *state)
{
    ClearScreen(renderer, v4(0, 0, 0, 255));
    RenderImage(renderer, state->text[0], 
                v4(WINDOW_WIDTH / 2 - 180, 175, 360, 240));

    ui ui = CreateUI(renderer, platform);
    {
        v4 rect = v4(WINDOW_WIDTH / 2 - 120, 400, 240, 80);
        if (Button(&ui, rect, state->text[1]))
        {
            state->current_mode = MODE_freeplay;
        }
    }

}

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
                    // Skips PIECE_white and PIECE_black
                    if (i != 7 && i != 14)
                    {
                        printf("Error: %d not loaded\n", i);
                    }
                } break;
            }
        }

        state->text[0] = IMG_LoadTexture(renderer, "data/chess.png");
        state->text[1] = IMG_LoadTexture(renderer, "data/white_play.png");

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

        state->castle.black_can_castle_left = 1;
        state->castle.black_can_castle_right = 1;
        state->castle.white_can_castle_left = 1;
        state->castle.white_can_castle_right = 1;

        platform->initialized = 1;
    }

    if (state->current_mode != MODE_menu)
    {
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
                        new_move.moved_piece = state->current_selected.type;
                        new_move.moved_to_i = i;
                        new_move.moved_to_j = j;
                        new_move.taken_piece = state->board[j][i];
                        new_move.original_i = state->current_selected.i;
                        new_move.original_j = state->current_selected.j;
                    }

                    if (CheckMoveValidity(state, new_move))
                    {
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

        if (platform->mouse_right_up && !state->current_selected.set)
        {
            if (!Empty(state->moves))
            {
                move undo = Pop(state->moves);
                if (undo.moved_piece == PIECE_black_king && undo.moved_to_i == 6) 
                {
                    UndoMove(state, undo);
                    state->castle.black_can_castle_right = 1;
                    state->board[0][7] = PIECE_black_rook;
                    state->board[0][5] = PIECE_none;
                }
                else if (undo.moved_piece == PIECE_black_king && undo.moved_to_i == 2) 
                {
                    UndoMove(state, undo);
                    state->castle.black_can_castle_left = 1;
                    state->board[0][0] = PIECE_black_rook;
                    state->board[0][3] = PIECE_none;
                }
                else if (undo.moved_piece == PIECE_white_king && undo.moved_to_i == 6) 
                {
                    UndoMove(state, undo);
                    state->castle.white_can_castle_right = 1;
                    state->board[7][7] = PIECE_white_rook;
                    state->board[7][5] = PIECE_none;
                }
                else if (undo.moved_piece == PIECE_white_king && undo.moved_to_i == 2) 
                {
                    UndoMove(state, undo);
                    state->castle.white_can_castle_left = 1;
                    state->board[7][0] = PIECE_white_rook;
                    state->board[7][3] = PIECE_none;
                }
                else
                {
                    UndoMove(state, undo);
                }
            }

            platform->mouse_down = 0; 
            platform->mouse_right_up = 0;
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

    if (state->current_mode == MODE_menu)
    {
        UpdateMenu(renderer, platform, state);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}