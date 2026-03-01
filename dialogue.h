#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

struct TextObject {
    SDL_Texture* texture;
    SDL_FRect dst;
};

struct SDLState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<TextObject> texts;
    bool typing = false;
};

void drawText(SDLState& state, const char* text, const char* font = "fonts/arial.ttf", int fontsize = 32, SDL_Color color = { 255,255,255,255 }, int sx = 25, int sy = 25, bool del = true);

void sDrawText(SDLState& state, const char* text, const char* font = "fonts/arial.ttf", int fontsize = 32, SDL_Color color = { 255,255,255,255 }, int sx = 25, int sy = 25, bool del = true, int speed = 50);

// NPC dialogue
void npcGreeting(SDLState& state);
void npcShopkeeper(SDLState& state);
// add more NPCs here...