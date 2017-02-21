#include <SDL.h>
#include <time.h>
#include <conio.h>
#include "mysdl.h"
#include "debug.h"
#include "world.h"
#include "release.h"
#include "agent.h"


int main (int argc, char* args[]) {
    srand(time(NULL));
    bool show_background = true;
    SDL_Event event;
    if (init("EVOLUTION SIMULATOR")) {
        warning ("MAIN THREAD", "error on initializing, terminating process");
        return 0;
    }
    init_food();
    init_agents();
    for (bool quit = false; quit == false; ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else
            if (event.type == SDL_KEYDOWN)
                switch (event.key.keysym.sym) {
                        // do something
                    case SDLK_i:
                        agent[0].f_left  = !agent[0].f_left;
                        break;
                    case SDLK_p:
                        agent[0].f_right = !agent[0].f_right;
                        break;
                    case SDLK_r:    // randomize food
                        init_food();
                        break;
                    case SDLK_h:    // show background
                        show_background = !show_background;
                        break;
                    case SDLK_e:
                        eat(&agent[0]);
                        break;
                    case SDLK_SPACE:
                        agent[0].boost_strenght = 30;
                        break;
                }
        }
        // rederer background color
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        if (show_background)
            printworld();
        //printg(image, 10, 10);
        move_agents();
        print_agents();
        SDL_RenderPresent(renderer);
        SDL_Delay(PAUSE_DELAY);
    }

    //SDL_DestroyImage(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    renderer = NULL;
    SDL_Quit();
    return 0;
}
