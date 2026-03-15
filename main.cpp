#include <SDL.h>
#include <SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <thread>
#include <iostream>
#include "dialogue.h"
using namespace std;
void cleanup(SDLState& state);
void entername(SDLState& state);
struct Player {
    char name[50] = {};
    char imagel[99] = {};
    float x = 0;
    float y = 0;
};
int main(int argc, char* argv[]) {
    SDLState state;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        return 1;
    }
    state.winwidth = 800;
    state.winheight = 600;
    state.window = SDL_CreateWindow("SDL3 Demo", state.winwidth, state.winheight, 0);
    if (!state.window) { cleanup(state); return 1; }
    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer) { cleanup(state); return 1; }
    if (!TTF_Init()) { cleanup(state); return 1; }
    std::vector<CollisionObject> walls(1);
    walls[0].size = { (float)0,(float)550,(float)800,(float)100 };
    SDL_Color col = { 255, 255, 255, 255 };
    thread t(entername, ref(state));
    t.detach();
    Player player1;
    SDL_Texture* itexture = nullptr;
    SDL_FRect irect = {};
    strcat_s(player1.imagel, "icon.png");
    SDL_Surface* isurface = IMG_Load(player1.imagel);
    player1.x = (float)(state.winwidth / 2) - (isurface->w / 2.0f);
    player1.y = (float)(state.winheight / 2) - (isurface->h / 2.0f);
    int movex = 0;
    int speed = 3;

    itexture = SDL_CreateTextureFromSurface(state.renderer, isurface);
    irect = {
        (float)(state.winwidth / 2) - (isurface->w / 2.0f),
        (float)(state.winheight / 2) - (isurface->h / 2.0f),
        (float)isurface->w,
        (float)isurface->h
    };
    SDL_FRect oldirect = irect;
    SDL_DestroySurface(isurface);
    bool running = true;
    float velocity = 0.0f;
    float gravity = 0.01f;
    bool grounded = false;

    while (running) {
        SDL_Event event{ 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                state.typing = false;
                running = false;
                break;
            case SDL_EVENT_TEXT_INPUT:
                if (state.enteringname) {
                    if (state.allowtyping) {
                        strcat_s(state.inputText, event.text.text);
                        drawText(state, state.inputText, "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 300, true, true);
                        drawText(state, "Please Enter Your Name:", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, state.winheight / 2 - 100, false, true);
                    }
                }
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_RETURN && state.allowtyping) {
                    if (state.enteringname) {
                        if (strlen(state.inputText) != 0) {
                            strcpy_s(player1.name, state.inputText);
                        }
                        else {
                            strcpy_s(player1.name, "John Doe");
                        }
                        state.allowtyping = false;
                        state.enteringname = false;
                        std::cout << player1.name;
                        for (auto& t : state.texts)
                            SDL_DestroyTexture(t.texture);
                    }
                }
                if ((event.key.key == SDLK_W || event.key.key == SDLK_SPACE) && grounded) {
                    velocity = -2.5f;
                    grounded = false;
                }
                if (event.key.key == SDLK_BACKSPACE && strlen(state.inputText) > 0 && strlen(state.inputText) < 256 && state.allowtyping) {
                    if (state.enteringname) {
                        state.inputText[strlen(state.inputText) - 1] = '\0';
                        if (strlen(state.inputText) != 0) {
                            drawText(state, state.inputText, "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 300, true, true);
                        }
                        else {
                            drawText(state, " ", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 300, true, true);
                        }
                        drawText(state, "Please Enter Your Name:", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, state.winheight / 2 - 100, false, true);
                    }
                }
                break;
            }
        }
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);
        if (!state.enteringname) {
            SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
            SDL_RenderFillRects(state.renderer, &walls[0].size, 1);
            const bool* keys = SDL_GetKeyboardState(NULL);

            movex = 0;
            if (keys[SDL_SCANCODE_A]) movex = -1;
            if (keys[SDL_SCANCODE_D]) movex = 1;

            velocity += gravity;

            oldirect.x = irect.x;
            oldirect.y = irect.y;
            irect.x += movex * speed;
            irect.y += velocity;

            grounded = false;
            for (auto& w : walls) {
                auto& ws = w.size;
                if (ws.w == 0 || ws.h == 0) continue;

                bool overlapX = irect.x < ws.x + ws.w && irect.x + irect.w > ws.x;
                bool overlapY = irect.y < ws.y + ws.h && irect.y + irect.h > ws.y;
                bool oldOverlapX = oldirect.x < ws.x + ws.w && oldirect.x + irect.w > ws.x;
                bool oldOverlapY = oldirect.y < ws.y + ws.h && oldirect.y + irect.h > ws.y;

                if (overlapX && overlapY) {
                    if (!oldOverlapX) irect.x = oldirect.x;
                    if (!oldOverlapY) {
                        irect.y = oldirect.y;
                        velocity = 0;
                        grounded = true;
                    }
                }
            }

            player1.x = irect.x;
            player1.y = irect.y;
            SDL_RenderTexture(state.renderer, itexture, NULL, &irect);
        }
        for (auto& t : state.texts)
            SDL_RenderTexture(state.renderer, t.texture, NULL, &t.dst);
        SDL_RenderPresent(state.renderer);
    }
    cleanup(state);
    return 0;
}
void entername(SDLState& state) {
    state.enteringname = true;
    sDrawText(state, "Please Enter Your Name:", "fonts/arial.ttf", 32, { 255,255,255,255 }, 0, state.winheight / 2 - 100, true, 100, true);
    SDL_StartTextInput(state.window);
    state.allowtyping = true;
}
void cleanup(SDLState& state) {
    for (auto& t : state.texts)
        SDL_DestroyTexture(t.texture);
    state.texts.clear();
    TTF_Quit();
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}