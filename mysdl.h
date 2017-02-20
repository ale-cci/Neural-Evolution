#pragma once

#include <SDL.h>
#include <string>

const static int SCREEN_WIDTH = 1024;   // 32 * 32
const static int SCREEN_HEIGHT = 640;   // 32 * 20

extern SDL_Window* gWindow;
extern SDL_Renderer* renderer;

struct SDL_Image {
    SDL_Texture* texture;
    int32_t width;
    int32_t height;
};

bool init(std::string TITLE = "BLANK");
SDL_Image* load(std::string IMAGE);
bool printg(SDL_Image* IMAGE, int16_t X, int16_t Y);
void SDL_DestroyImage(SDL_Image* IMAGE);
