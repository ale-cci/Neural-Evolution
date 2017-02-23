#include <iostream>
#include "generic_functions.h"

void warning(std::string id, std::string msg) {
    std::cout << id << "-" << msg << std::endl;
    return ;
}

int rand(int LOWER, int UPPER) {
    return (rand() % (UPPER - LOWER + 1 )) + LOWER;
}

void printagent(AGENT* _agent) {
    SDL_SetTextureColorMod(agent_texture.texture, AGENT_RED, AGENT_GREEN, AGENT_BLUE);
    SDL_Rect rekt = {(int) getcenter(_agent).X - agent_texture.width/2,(int) getcenter(_agent).Y - agent_texture.height/2, agent_texture.width, agent_texture.height};
    SDL_Point pnt = {agent_texture.width/2, agent_texture.height/2};
    SDL_RenderCopyEx(renderer, agent_texture.texture,NULL, &rekt, _agent->rotation, &pnt , SDL_FLIP_NONE);
    //printg(&agent_texture, agent->X, agent->Y);
}
