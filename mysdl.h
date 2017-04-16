#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#define SDL_YELLOW  {0xff, 0xff, 0, 0}
#define SDL_RED     {0xff, 0, 0, 0}
#define SDL_BLUE    {0, 0, 0xff, 0}
#define SDL_GREEN   {0, 0xff, 0, 0}
#define SDL_WHITE   {0xff, 0xff, 0xff, 0}
#define SDL_PINK    {0xff, 0, 0xff, 0}

const static int SCREEN_WIDTH = 1024;   // 32 * 32
const static int SCREEN_HEIGHT = 640;   // 32 * 20

extern SDL_Window* gWindow;
extern TTF_Font* font;
extern SDL_Renderer* renderer;

struct SDL_Image {
    SDL_Texture* texture;
    int32_t width;
    int32_t height;
};

bool init(std::string TITLE = "BLANK");
SDL_Image* load(std::string IMAGE);
SDL_Image* load_trsp(std::string IMAGE, SDL_Color BACKGROUND = {0, 0xFF, 0});
bool printg(SDL_Image* IMAGE, int16_t X, int16_t Y);
void SDL_DestroyImage(SDL_Image* IMAGE);
bool write(const uint16_t Coord_X, const uint16_t Coord_Y, const char* STRING, ...);
