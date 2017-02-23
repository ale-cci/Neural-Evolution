#include <iostream>
#include "generic_functions.h"
#include "world.h"
#include "agent.h"

void warning(std::string id, std::string msg) {
    std::cout << id << " " << msg << std::endl;
    return ;
}

int rand(int LOWER, int UPPER) {
    return (rand() % (UPPER - LOWER + 1 )) + LOWER;
}

double rand(double LOWER, double UPPER) {
    double _ret = (double) rand() / RAND_MAX;
    return _ret * (UPPER - LOWER) + LOWER;
}

void printagent(struct AGENT * _agent) {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0);
    for (int y = -agent_texture.width/2; y <= agent_texture.width/2; ++y)
        for (int x = -agent_texture.width/2; x <= agent_texture.width/2; ++x)
            if ((x * x + y*y <= agent_texture.width * agent_texture.width / 4) && (x * x + y*y >= (agent_texture.width -2)*(agent_texture.width-2)/ 4))
                SDL_RenderDrawPoint(renderer, getcenter(_agent).X + x, getcenter(_agent).Y + y);
}
