#include "render.h"
#include "../game/game.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

int initSDLComponents(SDLComponents &sdl)
{
    // init and verifiy
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "sdl init error" << SDL_GetError() << std::endl;
        return -1;
    }

    // create the window that is shown
    sdl.window = SDL_CreateWindow(
        "Chess game",
        SDL_WINDOWPOS_CENTERED, // open centered
        SDL_WINDOWPOS_CENTERED, // open centered
        1920,                   // width
        1080,                   // height
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    // verify
    if (!sdl.window)
    {
        std::cerr << "window error" << std::endl;
        SDL_Quit();
        return -1;
    }

    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);

    return 0;
}

SDL_Texture *loadTexture(SDLComponents &sdl, const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        std::cerr << "failed to load image: " << path << IMG_GetError() << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest); // make loaded textures look sharper

    return texture;
}

void loadAllPieceTextures(SDLComponents &sdl, PieceTextures &textures)
{
    textures.textures[WHITE][PAWN] = loadTexture(sdl, "assets/pieces/white_pawn.png");
    textures.textures[WHITE][KNIGHT] = loadTexture(sdl, "assets/pieces/white_knight.png");
    textures.textures[WHITE][ROOK] = loadTexture(sdl, "assets/pieces/white_rook.png");
    textures.textures[WHITE][BISHOP] = loadTexture(sdl, "assets/pieces/white_bishop.png");
    textures.textures[WHITE][QUEEN] = loadTexture(sdl, "assets/pieces/white_queen.png");
    textures.textures[WHITE][KING] = loadTexture(sdl, "assets/pieces/white_king.png");

    textures.textures[BLACK][PAWN] = loadTexture(sdl, "assets/pieces/black_pawn.png");
    textures.textures[BLACK][KNIGHT] = loadTexture(sdl, "assets/pieces/black_knight.png");
    textures.textures[BLACK][ROOK] = loadTexture(sdl, "assets/pieces/black_rook.png");
    textures.textures[BLACK][BISHOP] = loadTexture(sdl, "assets/pieces/black_bishop.png");
    textures.textures[BLACK][QUEEN] = loadTexture(sdl, "assets/pieces/black_queen.png");
    textures.textures[BLACK][KING] = loadTexture(sdl, "assets/pieces/black_king.png");
}

void destroyAllPieceTextures(PieceTextures &textures)
{
    for (int color = 0; color < 2; color++)
    {
        for (int piece = 0; piece < 6; piece++)
        {
            if (textures.textures[color][piece])
            {
                SDL_DestroyTexture(textures.textures[color][piece]);
            }
        }
    }
}

void renderBoard(SDLComponents &sdl, const Game &game, PieceTextures &textures)
{
    int windowWidth;
    int windowHeight;
    SDL_GetWindowSize(sdl.window, &windowWidth, &windowHeight);

    int boardSize = (windowHeight < windowWidth) ? windowHeight : windowWidth;
    int renderedSquareSize = boardSize / 8;

    // fixes rounding errors
    int actualBoardSize = renderedSquareSize * 8;

    int windowWidthPadding = (windowWidth - actualBoardSize) / 2;
    int windowHeightPadding = (windowHeight - actualBoardSize) / 2;

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {

            SDL_Rect renderedSquare = {(windowWidthPadding + (renderedSquareSize * col)), (windowHeightPadding + (renderedSquareSize * row)), renderedSquareSize, renderedSquareSize};

            // alternate
            if ((row + col) % 2 == 0)
            {
                SDL_SetRenderDrawColor(sdl.renderer, 240, 217, 181, 255);
            }

            else
            {
                SDL_SetRenderDrawColor(sdl.renderer, 181, 136, 99, 255);
            }

            SDL_RenderFillRect(sdl.renderer, &renderedSquare);

            if (game.selectedSquare)
            {
                if ((game.selectedSquare->row == row) && (game.selectedSquare->col == col))
                {
                    SDL_SetRenderDrawColor(sdl.renderer, 0, 255, 0, 0);
                    renderSelectedSquare(sdl.renderer, renderedSquare, 4);
                }
            }

            if (game.board.squares[row][col].occupied)
            {
                Piece piece = game.board.squares[row][col].piece;
                renderPiece(sdl, textures.textures[piece.color][piece.type], renderedSquare.x, renderedSquare.y, renderedSquareSize);
            }
        }
    }
}

void renderPiece(SDLComponents &sdl, SDL_Texture *texture, int x, int y, int size)
{
    SDL_Rect posRect = {x, y, size, size};
    SDL_RenderCopy(sdl.renderer, texture, nullptr, &posRect);
}

void renderSelectedSquare(SDL_Renderer *renderer, SDL_Rect rect, int thickness)
{
    SDL_Rect top = {rect.x, rect.y, rect.w, thickness};
    SDL_RenderFillRect(renderer, &top);

    SDL_Rect bottom = {rect.x, rect.y + rect.h - thickness, rect.w, thickness};
    SDL_RenderFillRect(renderer, &bottom);

    SDL_Rect left = {rect.x, rect.y, thickness, rect.h};
    SDL_RenderFillRect(renderer, &left);

    SDL_Rect right = {rect.x + rect.w - thickness, rect.y, thickness, rect.h};
    SDL_RenderFillRect(renderer, &right);
}