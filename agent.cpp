#include <cstdlib>
#include "debug.h"
#include "release.h"
#include "mysdl.h"
#include "agent.h"
SDL_Image buddy;
SDL_Image* agent_texture = &buddy;

bool init_agent(std::string PATH) {
    SDL_Surface* tmp = SDL_LoadBMP(PATH.c_str());
    if (tmp == NULL)
        return EXIT_FAILURE;
    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 0, 0xFF, 0));

    agent_texture->texture = SDL_CreateTextureFromSurface(renderer, tmp);
    agent_texture->width = tmp->w;
    agent_texture->height = tmp->h;

    SDL_SetTextureColorMod(agent_texture->texture, AGENT_RED, AGENT_GREEN, AGENT_BLUE);
    if (agent_texture->texture == NULL)
        warning("LOADAGENT", "error converting surface to texture");
    SDL_FreeSurface(tmp);
    return EXIT_SUCCESS;
}

void printagent(AGENT* agent) {
    printg(agent_texture, agent->X, agent->Y);
}
