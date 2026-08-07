#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

struct CollisionObject {
    SDL_FRect size{ 0, 0, 0, 0 };
    SDL_Color color{ 255, 255, 255, 255 };
};

struct TextObject {
    std::string id = "";
    SDL_Texture* texture = nullptr;
    SDL_FRect dst{};
};

struct SDLState {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int winwidth = 800;
    int winheight = 600;

    bool enteringname = false;
    bool allowtyping = false;
    bool typing = false;
    char inputText[256] = "";

    std::vector<TextObject> texts;
    std::string typeFullText = "";
    std::string typeFont = "";
    int typeFontSize = 32;
    SDL_Color typeColor = { 255, 255, 255, 255 };
    int typeX = 0;
    int typeY = 0;
    bool typeCenter = false;

    size_t typeIndex = 0;
    float typeTimer = 0.0f;
    float typeSpeed = 0.05f;
};

void drawText(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, const char* tag = "ui", bool center = false);
void startTypewriter(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, float speedInSeconds = 0.05f, bool center = false);
void updateTypewriter(SDLState& state, float deltaTime);