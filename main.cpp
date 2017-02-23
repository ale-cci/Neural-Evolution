#include <SDL.h>
#include <time.h>
#include <conio.h>
#include <iostream>
#include <cstdlib>

#include "mysdl.h"
#include "generic_functions.h"
#include "world.h"
#include "generic_functions.h"


int main (int argc, char* args[]) {
    srand(time(NULL));
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
                        // do something
                    case SDLK_i:
                        agent[0].f_left  = (!agent[0].f_left) * 5;
                        break;
                    case SDLK_p:
                        agent[0].f_right = (!agent[0].f_right) * 5;
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
        SDL_SetRenderDrawColor(renderer, 38, 38, 38, 255);
        SDL_RenderClear(renderer);
        if (show_background)
            printworld();

        move_agents();
        //std::cout << agent[0].X << " " << agent[0].Y << std::endl;
        //moveagent(&agent[0]);
        print_agents();
        give_agent_input(&agent[0]);
        //printagent(&agent[1]);
        //printagent(&agent[0]);
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
