#include "chess.h"

internal void ClearScreen(SDL_Renderer *renderer, u8 r, u8 g, u8 b, u8 a) 
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

internal void RenderSelectedPiece(SDL_Renderer *renderer, game_state *state)
{
    int w, h;
    SDL_QueryTexture(state->textures[state->current_selected.piece], NULL, NULL, &w, &h);
    SDL_Rect rect = { state->input.mouse_x - CELL_HEIGHT / 2, 
                      state->input.mouse_y - CELL_HEIGHT / 2,  
                      CELL_WIDTH - 10, CELL_HEIGHT - 10 };
    SDL_RenderCopy(renderer, state->textures[state->current_selected.piece], NULL, &rect);
}

internal void RenderPiece(SDL_Renderer *renderer, game_state *state, int i, int j, pieces piece)
{
    if (piece != PIECE_none)
    {
        int w, h;
        SDL_QueryTexture(state->textures[piece], NULL, NULL, &w, &h);
        SDL_Rect rect = { i * CELL_WIDTH + 5, j * CELL_WIDTH + 5, CELL_WIDTH - 10, CELL_HEIGHT - 10};
        SDL_RenderCopy(renderer, state->textures[piece], NULL, &rect);
    }
}

internal void RenderBoard(SDL_Renderer *renderer, game_state *state)
{
    for (int j = 0; j < BOARD_HEIGHT; ++j)
    {
        for (int i = 0; i < BOARD_WIDTH; ++i)
        {
            if (i % 2 == j % 2)
            {
                SDL_SetRenderDrawColor(renderer, 81, 42, 42, 255);
                SDL_Rect rect = { i * CELL_WIDTH, j * CELL_HEIGHT, 
                                CELL_WIDTH, CELL_HEIGHT };
                SDL_RenderFillRect(renderer, &rect);
            }
            else 
            {
                SDL_SetRenderDrawColor(renderer, 124, 76, 64, 255);
                SDL_Rect rect = { i * CELL_WIDTH, j * CELL_HEIGHT, 
                                CELL_WIDTH, CELL_HEIGHT };
                SDL_RenderFillRect(renderer, &rect);
            }

            RenderPiece(renderer, state, i, j, state->board[j][i]);
        }
    }
}

internal void SetPieceSelected(game_state *state)
{
    int i = (state->input.mouse_x - state->input.mouse_x % CELL_WIDTH) / CELL_WIDTH; 
    int j = (state->input.mouse_y - state->input.mouse_y % CELL_HEIGHT) / CELL_HEIGHT; 

    if (((state->current_turn == TURN_white && 
        (state->board[j][i] <= PIECE_white_king && state->board[j][i] >= PIECE_white_pawn)) ||
        (state->current_turn == TURN_black &&
        (state->board[j][i] <= PIECE_black_king && state->board[j][i] >= PIECE_black_pawn))) && 
        !state->current_selected.set)
    {
        state->current_selected.set = 1;
        state->current_selected.piece = state->board[j][i];
        state->current_selected.i = i; 
        state->current_selected.j = j; 
        state->board[j][i] = PIECE_none;
    }
}

internal void UpdateApp(SDL_Renderer *renderer, game_state *state)
{
    if (!state->board_initialized)
    {
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
        memcpy(state->board[0], 
              (pieces [8]) { PIECE_black_rook, PIECE_black_knight, PIECE_black_bishop, PIECE_black_queen, PIECE_black_king, PIECE_black_bishop, PIECE_black_knight, PIECE_black_rook }, 
               BOARD_WIDTH * 4);
        memcpy(state->board[1], 
               (pieces [8]){ PIECE_black_pawn, PIECE_black_pawn, PIECE_black_pawn, PIECE_black_pawn, PIECE_black_pawn, PIECE_black_pawn, PIECE_black_pawn, PIECE_black_pawn }, 
               BOARD_WIDTH * 4);
        memcpy(state->board[6], 
               (pieces [8]){ PIECE_white_pawn, PIECE_white_pawn, PIECE_white_pawn, PIECE_white_pawn, PIECE_white_pawn, PIECE_white_pawn, PIECE_white_pawn, PIECE_white_pawn },
               BOARD_WIDTH * 4);
        memcpy(state->board[7],
               (pieces [8]){ PIECE_white_rook, PIECE_white_knight, PIECE_white_bishop, PIECE_white_queen, PIECE_white_king, PIECE_white_bishop, PIECE_white_knight, PIECE_white_rook },
               BOARD_WIDTH * 4);

        state->current_turn = TURN_white; 

        state->board_initialized = 1;
    }

    ClearScreen(renderer, 0, 0, 0, 255);
    RenderBoard(renderer, state);

    if (state->input.mouse_down)
    {
        SetPieceSelected(state);
        RenderSelectedPiece(renderer, state);
    }

    if (state->input.mouse_up)
    {
        if (state->current_selected.piece != PIECE_none)
        {
            int i = (state->input.mouse_x - state->input.mouse_x % CELL_WIDTH) / CELL_WIDTH; 
            int j = (state->input.mouse_y - state->input.mouse_y % CELL_HEIGHT) / CELL_HEIGHT; 
            state->board[j][i] = state->current_selected.piece;
            state->current_turn = (state->current_turn == TURN_white) ? TURN_black : TURN_white;
        }

        state->current_selected.piece = PIECE_none;
        state->current_selected.set = 0;
        state->input.mouse_up = 0;
    }

    if (state->current_selected.piece == PIECE_none)
    {
        state->current_selected.set = 0;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}