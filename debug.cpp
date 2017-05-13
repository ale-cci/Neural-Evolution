#include <iostream>
#include <cstdio>
#include "generic_functions.h"
#include "world.h"
#include "agent.h"

std::string texture_path = "IMAGES/debug01.bmp";

void warning(std::string id, std::string msg, ...) {
    va_list li;
    va_start (li, msg);
    vprintf(std::string(id + " " + msg + "\n").c_str(), li);
    va_end(li);
    return ;
}

void printagent(const uint16_t id) {
    write(agent[id].X -5, agent[id].Y-3, "%d", id);
    SDL_SetRenderDrawColor(renderer, 0, 0xff, 0, 0);
    SDL_RenderDrawLine(renderer, agent[id].X+2, agent[id].Y+3, agent[id].X + agent[id].energy*32/MAX_HEALTH +2, agent[id].Y+3);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0);
    SDL_RenderDrawLine(renderer, agent[id].X+2, agent[id].Y+5, agent[id].X + agent[id].food_bar*32/MAX_FOOD_BAR+2, agent[id].Y+5);
    if (id == triggered)
        SDL_SetRenderDrawColor(renderer, 0, 0xff, 0xff, 0);
    else
        SDL_SetRenderDrawColor(renderer, 218, 22, 58, 0);
    for (int y = -agent_texture.width/2; y <= agent_texture.width/2; ++y)
        for (int x = -agent_texture.width/2; x <= agent_texture.width/2; ++x)
            if ((x * x + y*y <= agent_texture.width * agent_texture.width / 4) && (x * x + y*y >= (agent_texture.width -2)*(agent_texture.width-2)/ 4))
                SDL_RenderDrawPoint(renderer, int(getcenter(id).X + x) % (SCREEN_WIDTH-1), int(getcenter(id).Y + y) % (SCREEN_HEIGHT-1));

    SDL_Rect rekt;
    rekt.h = agent_texture.height+1;
    rekt.w = agent_texture.width+1;
    rekt.x = agent[id].X;
    rekt.y = agent[id].Y;

    SDL_Point p = {getcenter(id).X, getcenter(id).Y};
    //SDL_RenderCopyEx(renderer, agent_texture.texture, NULL, &rekt, -agent[id].rotation, NULL, SDL_FLIP_NONE);
}
