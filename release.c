#include <iostream>
#include <fstream>
#include "generic_functions.h"
#include "world.h"
#include "agent.h"

std::string texture_path = "IMAGES/debug01.bmp";

void warning(std::string id, std::string msg, ...) {
    std::ofstream out("crush.log");
    out << id << "-" << msg << std::endl;
    out.close();
    return ;
}


/// TODO print graphics
void printagent(const uint16_t id) {

    write(agent[id].X -5, agent[id].Y-3, "%d", id);
    SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0);
    SDL_RenderDrawLine(renderer, agent[id].X+2, agent[id].Y+3, agent[id].X + agent[id].energy +2, agent[id].Y+3);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0);
    SDL_RenderDrawLine(renderer, agent[id].X+2, agent[id].Y+5, agent[id].X + 0.32*agent[id].food_bar+2, agent[id].Y+5);
    if (id == triggered)
        SDL_SetRenderDrawColor(renderer, 0, 0xff, 0xff, 0);
    else
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0);
    for (int y = -agent_texture.width/2; y <= agent_texture.width/2; ++y)
        for (int x = -agent_texture.width/2; x <= agent_texture.width/2; ++x)
            if ((x * x + y*y <= agent_texture.width * agent_texture.width / 4) && (x * x + y*y >= (agent_texture.width -2)*(agent_texture.width-2)/ 4))
                SDL_RenderDrawPoint(renderer, int(getcenter(id).X + x) % (SCREEN_WIDTH-1), int(getcenter(id).Y + y) % (SCREEN_HEIGHT-1));
}
