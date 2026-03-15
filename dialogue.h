#pragma once
#include <SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
struct TextObject {
    SDL_Texture* texture;
    SDL_FRect dst;
};
struct CollisionObject {
    SDL_FRect size;
    bool collide = true;
};
struct SDLState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<TextObject> texts;
    bool typing = false;
    int winwidth;
    int winheight;
    bool allowtyping = false;
    char inputText[256] = {};
    bool enteringname = false;
};
void drawText(SDLState& state, const char* text, const char* font = "fonts/arial.ttf", int fontsize = 32, SDL_Color color = { 255,255,255,255 }, int sx = 25, int sy = 25, bool del = true, bool center = false);
void sDrawText(SDLState& state, const char* text, const char* font = "fonts/arial.ttf", int fontsize = 32, SDL_Color color = { 255,255,255,255 }, int sx = 25, int sy = 25, bool del = true, int speed = 50, bool center = false);