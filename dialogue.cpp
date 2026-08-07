#include "dialogue.h"
#include <iostream>
#include <cstring>

void drawText(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, const char* tag, bool center) {
    for (auto it = state.texts.begin(); it != state.texts.end(); ) {
        if (it->id == tag) {
            if (it->texture) SDL_DestroyTexture(it->texture);
            it = state.texts.erase(it);
        } else {
            ++it;
        }
    }

    if (!text || strlen(text) == 0) return;

    TTF_Font* f = TTF_OpenFont(font, (float)fontsize);
    if (!f) return;

    SDL_Surface* surface = TTF_RenderText_Solid(f, text, 0, color);
    if (!surface) {
        TTF_CloseFont(f);
        return;
    }

    TextObject obj;
    obj.id = tag;
    int w, h;
    TTF_GetStringSize(f, text, 0, &w, &h);
    obj.texture = SDL_CreateTextureFromSurface(state.renderer, surface);

    float posX = center ? ((state.winwidth / 2.0f) - (w / 2.0f)) : (float)sx;
    obj.dst = { posX, (float)sy, (float)surface->w, (float)surface->h };

    SDL_DestroySurface(surface);
    TTF_CloseFont(f);
    state.texts.push_back(obj);
}

void startTypewriter(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, float speedInSeconds, bool center) {
    state.typeFullText = text;
    state.typeFont = font;
    state.typeFontSize = fontsize;
    state.typeColor = color;
    state.typeX = sx;
    state.typeY = sy;
    state.typeSpeed = speedInSeconds;
    state.typeCenter = center;

    state.typeIndex = 0;
    state.typeTimer = 0.0f;
    state.typing = true;

    drawText(state, "", font, fontsize, color, sx, sy, "prompt", center);
}

void updateTypewriter(SDLState& state, float deltaTime) {
    if (!state.typing || !state.enteringname) {
        state.typing = false;
        return;
    }

    state.typeTimer += deltaTime;

    if (state.typeTimer >= state.typeSpeed) {
        state.typeTimer = 0.0f;

        if (state.typeIndex < state.typeFullText.length()) {
            state.typeIndex++;

            std::string sub = state.typeFullText.substr(0, state.typeIndex);
            drawText(state, sub.c_str(), state.typeFont.c_str(), state.typeFontSize, state.typeColor, state.typeX, state.typeY, "prompt", state.typeCenter);
        } else {
            state.typing = false;
        }
    }
}