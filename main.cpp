#include <SDL.h>
#include <time.h>
#include <conio.h>
#include <iostream>
#include <cstdlib>

#include "mysdl.h"
#include "world.h"
#include "generic_functions.h"


int main (int argc, char* args[]) {
    time_t seed = time(NULL);
    srand(1487971779);
    std::cout << "SEED" << " " << seed << std::endl;
    bool show_background = false;
    SDL_Event event;

    if (init("EVOLUTION SIMULATOR")) {
        warning ("MAIN THREAD", "error on initializing, terminating process");
        return 0;
    }

    init_food();
    init_agents();
    agent[0].X = SCREEN_WIDTH/2;
    agent[0].Y = SCREEN_HEIGHT/2;

    for (bool quit = false; quit == false; ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else
            if (event.type == SDL_KEYDOWN)
                switch (event.key.keysym.sym) {
                        break;
                    case SDLK_r:    // randomize food
                        init_food();
                        break;
                    case SDLK_h:    // show background
                        show_background = !show_background;
                        break;
                    case SDLK_e:
                        crunch(0);
                        break;
                    case SDLK_SPACE:
                        agent[0].boost_strenght = 30;
                        break;
                }
        }
        // rederer background color
        SDL_SetRenderDrawColor(renderer, 38, 38, 38, 255);
        SDL_RenderClear(renderer);
        if (show_background)
            printworld();
        input_agents();
        execute_agents();
        output_agents();
        update_world();
        move_agents();
        print_agents();
        SDL_RenderPresent(renderer);
        SDL_Delay(PAUSE_DELAY);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    renderer = NULL;
    SDL_Quit();
    return 0;
}
