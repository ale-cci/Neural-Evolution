///http://www.wildml.com/2015/09/recurrent-neural-networks-tutorial-part-1-introduction-to-rnns/

#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include "mysdl.h"
#include "world.h"
#include "generic_functions.h"

int main (int argc, char* args[]) {
    time_t seed = 1492250131;// time(NULL);
    srand(seed);
    uint32_t status = 0;
    uint8_t togglefullscreen = true;
    std::cout << "SEED" << " " << seed << std::endl;
    bool show_background = false;
    SDL_Event event;

    if (init("EVOLUTION SIMULATOR")) {
        warning ("MAIN THREAD", "error on initializing, terminating process");
        return EXIT_FAILURE;
    }

    agent_texture = *load_trsp(texture_path.c_str());
    agent_texture.width = AGENT_RADIUS *2;
    SDL_SetWindowIcon(gWindow, IMG_Load("./IMAGES/evo.png"));
    init_agents();
    init_food();
    init_lastdeath();
    for (bool quit = false; quit == false; ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                triggered = -1;
                for (int i=0; i < POPULATION_COUNT; ++i) {
                    if (in_agent({double(x), double(y)}, i))
                        triggered = i;
                }

            }
            else
            if (event.type == SDL_DROPFILE) {

                std::ifstream in(event.drop.file, std::ios::binary);
                if (POPULATION_COUNT < MAX_POPULATION) {
                    insert_random_agent();
                    agent[POPULATION_COUNT-1].read(in);
                    triggered = POPULATION_COUNT-1;
                }
                else
                SDL_ShowSimpleMessageBox(
                        SDL_MESSAGEBOX_INFORMATION,
                        "SORRY, WE ARE FULL.",
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
                            for (int i=0; i < MAX_POPULATION; ++i) {
                                agent[i].write(out);
                            }
                            for (int y = 0; y < AREA_HEIGHT; ++y)
                                out.write((char*)area[y], sizeof(area[y]));
                            out.close();
                    }
                    else {
                        std::ifstream in(filename, std::ios::binary);
                        for (int i=0; i < MAX_POPULATION; ++i) {
                            agent[i].read(in);
                        }
                        for (int y = 0; y < AREA_HEIGHT; ++y)
                            in.read((char*)area[y], sizeof(area[y]));
                        in.close();
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
                        fast_forward = bound(fast_forward - 0.20, 0.10, 4);
                        break;
                    case SDLK_d:
                        fast_forward = bound(fast_forward + 0.20, 0.10, 4);
                        break;
                    case SDLK_j:
                        agent[0].rotation++;
                        break;
                    case SDLK_l:
                        agent[0].rotation--;
                        break;
                    case SDLK_e:
                        {
                            if (triggered == -1)
                                break;
                            char filename[64];
                            firstfree(filename, "agent%d.agt");
                            warning("MAIN", filename);
                            std::ofstream out(filename, std::ios::binary);
                            agent[triggered].write(out);
                            out.close();
                        }
                        break;
                    case SDLK_f:
                        if (togglefullscreen) {
                            status = (status)? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
                            SDL_SetWindowFullscreen(gWindow, status);
                            togglefullscreen = false;
                            SDL_GetWindowSize(gWindow, &REAL_WIDTH, &REAL_HEIGHT);
                        }
                        // FIXME toggle fullscreen
                        break;
                    case SDLK_SPACE:
                        bite(0,1);
                        break;
                    case SDLK_KP_0:
                        {
                            if (triggered == -1)
                                break;
                            std::ofstream out("LOG/info.log");
                            agent[triggered].brain.print(out);
                            out.close();

                        }
                        break;
                }
            }
            else
            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_f:
                        togglefullscreen = true;
                    break;
                }
            }
        }
        // rederer background color
        SDL_SetRenderDrawColor(renderer, 38, 38, 38, 255);
        SDL_RenderClear(renderer);
        if (show_background)
            printworld();
        //give_agent_input(0);
        update_world();
        SDL_RenderPresent(renderer);
        SDL_Delay(PAUSE_DELAY * (1/fast_forward));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    renderer = NULL;
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}
