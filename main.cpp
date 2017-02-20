#include <SDL.h>
#include <conio.h>
#include "mysdl.h"
#include "debug.h"
#include "world.h"
#include "release.h"

int main (int argc, char* args[]) {
    bool show_background = false;
    SDL_Event event;
    if (init()) {
        warning ("MAIN THREAD", "error on initializing, terminating process");
        return 0;
    }
    init_food();
    SDL_Image* image = load("IMAGES/default.bmp");
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
        if (image == NULL)
            warning("IMAGE", "IMAGE NOT LOADED");

        // rederer background color
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        printworld(show_background);
        //printg(image, 10, 10);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    SDL_DestroyImage(image);
    SDL_Quit();
    warning("QUITTING", "DONE");
    return 0;
}
