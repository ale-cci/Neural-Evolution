#include <SDL2/SDL.h>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "lib/logger.h"
#include "generic_functions.h"
#include "mysdl.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font;

bool init(std::string title) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        logger::log(logger::ERROR, "SDL_INITIALIZATION: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    gWindow = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL); // SDL_WINDOW_FULLSCREEN

    if (gWindow == NULL ) {
        logger::log(logger::CRITICAL, "SDL_CreateWindow(): %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (TTF_Init()) {
        logger::log(logger::CRITICAL, "TTF_Init(): %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
    font = TTF_OpenFont("FONT/cour.ttf" ,11);
    return EXIT_SUCCESS;
}


SDL_Image* load(std::string path) {
    SDL_Surface* tmp_surface = SDL_LoadBMP(path.c_str());
    SDL_Image *return_image;

    if (tmp_surface == NULL) {
        logger::log(logger::CRITICAL, "SDL_LoadBMP(): Unable to open image %s\n", path.c_str());
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

SDL_Image* load_trsp(std::string PATH, SDL_Color C) {
    SDL_Image* img = new SDL_Image;
    SDL_Surface* tmp = SDL_LoadBMP(PATH.c_str());

    if (tmp == NULL) {
        logger::log(logger::WARNING, "SDL_LoadBMP(): unable to open image %s\n", PATH.c_str());
        return NULL;
    }

    SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, C.r, C.g, C.b));

    img->texture = SDL_CreateTextureFromSurface(renderer, tmp);
    img->width = tmp->w;
    img->height = tmp->h;

    if (img->texture == NULL) {
        logger::log(logger::CRITICAL, "SDL_CreateTextureFromSurface(): %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FreeSurface(tmp);
    return img;
}

bool write(const uint16_t Coord_X, const uint16_t Coord_Y, const char* fmt, ...) {
    char _string[64];
    va_list li;
    va_start(li, fmt);
    vsprintf(_string, fmt, li);
    va_end(li);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, _string, SDL_WHITE);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = {Coord_X, Coord_Y, text_width, text_height};
    SDL_RenderCopy(renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
    return EXIT_SUCCESS;
}
