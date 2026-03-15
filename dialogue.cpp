#include "dialogue.h"
#include <iostream>
void drawText(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, bool del, bool center) {
    if (del) {
        for (auto& t : state.texts)
            SDL_DestroyTexture(t.texture);
        state.texts.clear();
    }
    TTF_Font* f = TTF_OpenFont(font, (float)fontsize);
    SDL_Surface* surface = TTF_RenderText_Solid(f, text, 0, color);
    TextObject obj;
    int w, h;
    TTF_GetStringSize(f, text, 0, &w, &h);
    obj.texture = SDL_CreateTextureFromSurface(state.renderer, surface);
    obj.dst = { ((float)sx * !center) + (center * ((state.winwidth / 2) - (w / 2))), (float)sy, (float)surface->w, (float)surface->h};
    SDL_DestroySurface(surface);
    TTF_CloseFont(f);
    state.texts.push_back(obj);
}
void sDrawText(SDLState& state, const char* text, const char* font, int fontsize, SDL_Color color, int sx, int sy, bool del, int speed, bool center) {
    state.typing = true;
    if (del) {
        for (auto& t : state.texts)
            SDL_DestroyTexture(t.texture);
        state.texts.clear();
    }
    TTF_Font* f = TTF_OpenFont(font, (float)fontsize);
    char ch[2] = { 0, 0 };
    int len = SDL_strlen(text);
    int totalw, h;
    TTF_GetStringSize(f, text, 0, &totalw, &h);
    int startx = center ? (state.winwidth / 2) - (totalw / 2) : sx;
    int curx = startx;
    for (int x = 0; x < len; x++) {
        if (!state.typing) break;
        ch[0] = text[x];
        int lw = 0, lh = 0;
        TTF_GetStringSize(f, ch, 0, &lw, &lh);
        drawText(state, ch, font, fontsize, color, curx, sy, false);
        curx += lw;  
        SDL_Delay(speed);
    }
    TTF_CloseFont(f);
    state.typing = false;
}