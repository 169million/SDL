#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <thread>
#include "dialogue.h"
using namespace std;

void cleanup(SDLState& state);

typedef struct {
    char name[50];
} Player;

int main(int argc, char* argv[]) {
    SDLState state;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        return 1;
    }

    state.window = SDL_CreateWindow("SDL3 Demo", 800, 600, 0);
    if (!state.window) { cleanup(state); return 1; }

    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer) { cleanup(state); return 1; }

    if (!TTF_Init()) { cleanup(state); return 1; }

    SDL_Color col = { 255, 255, 255, 255 };
    thread t(sDrawText, ref(state), "Hello, World!", "fonts/arial.ttf", 32, col, 25, 25, true, 100);
    t.detach();

    bool running = true;
    while (running) {
        SDL_Event event{ 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                state.typing = false;
                running = false;
                break;
            }
        }
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);
        for (auto& t : state.texts)
            SDL_RenderTexture(state.renderer, t.texture, NULL, &t.dst);
        SDL_RenderPresent(state.renderer);
    }

    cleanup(state);
    return 0;
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