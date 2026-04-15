#include "board/board.h"
#include "render/render.h"
#include "game/game.h"
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

int main()
{
    SDLComponents sdl;
    if (initSDLComponents(sdl) < 0)
    {
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // stuff for the loop
    bool running = true;
    SDL_Event event;

    // init game obj that holds all truth
    Game game;

    PieceTextures textures;
    loadAllPieceTextures(sdl, textures);

    // actual game loop
    while (running)
    {
        // check for events here
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int windowWidth;
                    int windowHeight;
                    SDL_GetWindowSize(sdl.window, &windowWidth, &windowHeight);
                    int mousePosX = event.button.x;
                    int mousePosY = event.button.y;
                    game.handleClick(mousePosX, mousePosY, windowWidth, windowHeight);
                    
                    // check game is over
                    if (game.gameWinner != NO_COLOR)
                    {
                        cout << game.gameWinner << " won the game!" << endl;
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(sdl.renderer, 120, 120, 120, 255);
        SDL_RenderClear(sdl.renderer); // sets background

        renderBoard(sdl, game, textures);

        SDL_RenderPresent(sdl.renderer); // show render
        SDL_Delay(16);                   // about 60 fps
    }

    destroyAllPieceTextures(textures);
    IMG_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();

    return 0;
}