#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Game;

struct SDLComponents
{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

struct PieceTextures
{
    SDL_Texture *textures[2][7];
};

int initSDLComponents(SDLComponents &sdl); // init renderer and window

SDL_Texture *loadTexture(SDLComponents &sdl, const char *path);
void loadAllPieceTextures(SDLComponents &sdl, PieceTextures &textures);
void destroyAllPieceTextures(PieceTextures &textures); // clear memory after use

void renderBoard(SDLComponents &sdl, const Game &game, PieceTextures &textures);
void renderPiece(SDLComponents &sdl, SDL_Texture *texture, int x, int y, int size);
void renderSelectedSquare(SDL_Renderer *renderer, SDL_Rect rect, int thickness);