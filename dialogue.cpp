#include "dialogue.h"

void drawText(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, bool del) {
    if (del) {
        for (auto& t : state.texts)
            SDL_DestroyTexture(t.texture);
        state.texts.clear();
    }
    TTF_Font* f = TTF_OpenFont(font, fontsize);
    SDL_Surface* surface = TTF_RenderText_Solid(f, text, 0, color);
    TextObject obj;
    obj.texture = SDL_CreateTextureFromSurface(state.renderer, surface);
    obj.dst = { (float)sx, (float)sy, (float)surface->w, (float)surface->h };
    SDL_DestroySurface(surface);
    TTF_CloseFont(f);
    state.texts.push_back(obj);
}
void sDrawText(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, bool del, int speed) {
    state.typing = true;
    if (del) {
        for (auto& t : state.texts)
            SDL_DestroyTexture(t.texture);
        state.texts.clear();
    }

    char ch[2] = { 0, 0 }; 
    int len = SDL_strlen(text);

    for (int x = 0; x < len; x++) {
        if (!state.typing) break;
        ch[0] = text[x];
        drawText(state, ch, font, fontsize, color, sx, sy, false);
        sx += fontsize; 
        SDL_Delay(speed);
    }
}

void npcGreeting(SDLState& state) {
    drawText(state, "Hello traveler!", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 25, true);
    drawText(state, "How can I help you?", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 60, false);
}

void npcShopkeeper(SDLState& state) {
    drawText(state, "Welcome to my shop!", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 25, true);
}