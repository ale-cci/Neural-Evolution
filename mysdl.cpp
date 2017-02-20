#include <SDL.h>
#include <cstdlib> // for exit
#include <string>
#include "debug.h"
#include "release.h"
#include "mysdl.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* renderer = NULL;

bool init(std::string title) {
    /*
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        warning("SDL INITIALIZATION", SDL_GetError());
        return EXIT_FAILURE;
    }
    */
    gWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (gWindow == NULL ) {
        warning("SDL WINDOW CREATION", SDL_GetError());
        return EXIT_FAILURE;
    }
    renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    return EXIT_SUCCESS;
}


SDL_Image* load(std::string IMAGE) {
    SDL_Surface* tmp_surface = SDL_LoadBMP(IMAGE.c_str());
    SDL_Image *return_image;

    if (tmp_surface == NULL) {
        IMAGE = "Unable to open image: " + IMAGE;
        warning("IMAGE LOAD", IMAGE.c_str());
        exit(1);
    }

    return_image->texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
    return_image->width = tmp_surface->w;
    return_image->height= tmp_surface->h;
    SDL_FreeSurface(tmp_surface);
    return return_image;
}

bool printg(SDL_Image* IMAGE, int16_t X, int16_t Y) {
    SDL_Rect rekt = {X, Y, IMAGE->width, IMAGE->height};
    SDL_RenderCopy(renderer, IMAGE->texture, NULL, &rekt);
    return EXIT_SUCCESS;
}

void SDL_DestroyImage(SDL_Image* IMAGE) {
    if (IMAGE->texture != NULL) {
        SDL_DestroyTexture(IMAGE->texture);
        IMAGE->width = 0;
        IMAGE->height = 0;
    }
}
