#include <SDL.h>
#include <conio.h>
#include "mysdl.h"
#include "debug.h"
#include "world.h"
#include "release.h"
#include "agent.h"


int main (int argc, char* args[]) {
    bool show_background = false;
    SDL_Event event;

    if (init("EVOLUTION SIMULATOR")) {
        warning ("MAIN THREAD", "error on initializing, terminating process");
        return 0;
    }
    init_food();
    init_agent("IMAGES/agent00.bmp");

    for (bool quit = false; quit == false; ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else
            if (event.type == SDL_KEYDOWN)
                switch (event.key.keysym.sym) {
                        // do something
                    case SDLK_r:    // randomize food
                        init_food();
                        break;
                    case SDLK_b:    // show background
                        show_background != show_background;
                        break;
                }
        }

        // rederer background color
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        printworld(show_background);
        //printg(image, 10, 10);
        SDL_RenderPresent(renderer);
        SDL_Delay(PAUSE_DELAY);
    }

    //SDL_DestroyImage(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    renderer = NULL;
    SDL_Quit();
    warning("QUITTING", "DONE");
    return 0;
}
