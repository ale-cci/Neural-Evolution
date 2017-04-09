#include <iostream>
#include "generic_functions.h"
#include "world.h"
#include "agent.h"

std::string texture_path = "IMAGES/debug01.bmp";

void warning(std::string id, std::string msg) {
    std::cerr << id << " " << msg << std::endl;
    return ;
}

void printagent(const uint16_t id) {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0);
    for (int y = -agent_texture.width/2; y <= agent_texture.width/2; ++y)
        for (int x = -agent_texture.width/2; x <= agent_texture.width/2; ++x)
            if ((x * x + y*y <= agent_texture.width * agent_texture.width / 4) && (x * x + y*y >= (agent_texture.width -2)*(agent_texture.width-2)/ 4))
                SDL_RenderDrawPoint(renderer, int(getcenter(id).X + x) % (SCREEN_WIDTH-1), int(getcenter(id).Y + y) % (SCREEN_HEIGHT-1));
}
