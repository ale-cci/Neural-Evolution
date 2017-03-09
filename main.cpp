///http://www.wildml.com/2015/09/recurrent-neural-networks-tutorial-part-1-introduction-to-rnns/

#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "mysdl.h"
#include "world.h"
#include "generic_functions.h"


int main (int argc, char* args[]) {
    time_t seed = time(NULL);
    srand(seed);

    std::cout << "SEED" << " " << seed << std::endl;
    bool show_background = false;
    SDL_Event event;

    if (init("EVOLUTION SIMULATOR")) {
        warning ("MAIN THREAD", "error on initializing, terminating process");
        return 0;
    }
    SDL_SetWindowIcon(gWindow, IMG_Load("./IMAGES/evo.png"));
    init_agents();
    init_food();
    SDL_Thread* food_t = SDL_CreateThread(t_init_food, "food_t", (void *)NULL);
    SDL_Thread* death_t = SDL_CreateThread(t_change_agent_status, "death_t", (void *)NULL);

    for (bool quit = false; quit == false; ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else
            if (event.type == SDL_DROPFILE) {
                SDL_ShowSimpleMessageBox(
                        SDL_MESSAGEBOX_INFORMATION,
                        "File dropped on window",
                        event.drop.file,
                        gWindow
                    );
            }
            else
            if (event.type == SDL_KEYDOWN) {
                const Uint8 *keystate = SDL_GetKeyboardState(NULL);
                bool shiftpressed = keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT];
                if ((event.key.keysym.sym >= SDLK_0) && (event.key.keysym.sym <= SDLK_9)) {
                    char filename[20];
                    sprintf(filename,"save%c.sav", event.key.keysym.sym);
                    std::cout << filename << std::endl;
                    if (shiftpressed) {
                            std::ofstream out(filename, std::ios::binary);
                            out.write((char*)agent, sizeof(agent));
                            out.close();
                    }
                    else {
                            std::ifstream out(filename, std::ios::binary);
                            out.read((char*)agent, sizeof(agent));
                            out.close();
                    }
                }
                switch (event.key.keysym.sym) {
                    case SDLK_h:    // show background
                        show_background = !show_background;
                        break;
                    case SDLK_i:
                        agent[0].f_left = (!agent[0].f_left)*MAX_STRENGHT;
                        break;
                    case SDLK_p:
                        agent[0].f_right = (!agent[0].f_right)*MAX_STRENGHT;
                        break;
                    case SDLK_s:
                        agent[0].rotation ++;
                        break;

                    case SDLK_SPACE:
                        break;
                }
            }
        }
        // rederer background color
        SDL_SetRenderDrawColor(renderer, 38, 38, 38, 255);
        SDL_RenderClear(renderer);
        if (show_background)
            printworld();
        input_agents();
        //give_agent_input(0);
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
