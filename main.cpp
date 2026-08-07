#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include "dialogue.h"
#include "Player.h"

using namespace std;

void cleanup(SDLState& state);
void entername(SDLState& state);

int main(int argc, char* argv[]) {
    SDLState state;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        return 1;
    }

    state.winwidth = 800;
    state.winheight = 600;

    state.window = SDL_CreateWindow("SDL3 Game", state.winwidth, state.winheight, SDL_WINDOW_RESIZABLE);
    if (!state.window) { cleanup(state); return 1; }

    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer) { cleanup(state); return 1; }

    SDL_SetRenderLogicalPresentation(state.renderer, 800, 600, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(state.renderer, 1);

    if (!TTF_Init()) { cleanup(state); return 1; }

    float genthick = 50.0f;
    std::vector<CollisionObject> walls(4);
    walls[0].size = { 0.0f, (float)state.winheight - genthick, (float)state.winwidth, genthick };
    walls[1].size = { 0.0f, 0.0f, genthick, (float)state.winheight };
    walls[2].size = { (float)state.winwidth - genthick, 0.0f, genthick, (float)state.winheight };
    walls[3].size = { (float)state.winwidth - genthick - 100.0f, 400.0f, genthick, 30.0f };
    walls[3].color = { 87, 87, 87, 255 };

    Player player1;
    player1.addAnimation(state.renderer, AnimState::IDLE, { "frame_blue.png" }, 0.15f, true);
    player1.addAnimation(state.renderer, AnimState::RUN, { "frame_red.png", "frame_green.png" }, 0.10f, true);
    player1.addAnimation(state.renderer, AnimState::JUMP, { "frame_yellow.png" }, 0.15f, false);

    bool running = true;
    bool initializedNamePrompt = false;

    Uint64 lastTime = SDL_GetPerformanceCounter();
    float deltaTime = 0.0f;

    while (running) {
        Uint64 now = SDL_GetPerformanceCounter();
        deltaTime = (float)(now - lastTime) / (float)SDL_GetPerformanceFrequency();
        lastTime = now;
        deltaTime = SDL_min(deltaTime, 0.05f);

        if (!initializedNamePrompt) {
            entername(state);
            initializedNamePrompt = true;
        }

        updateTypewriter(state, deltaTime);

        SDL_Event event{ 0 };
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                state.typing = false;
                running = false;
                break;

            case SDL_EVENT_TEXT_INPUT:
                if (state.enteringname && state.allowtyping) {
                    strncat(state.inputText, event.text.text, sizeof(state.inputText) - strlen(state.inputText) - 1);
                    drawText(state, state.inputText, "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 300, "user_input", true);
                }
                break;

            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    state.typing = false;
                    running = false;
                }

                if (event.key.key == SDLK_F11) {
                    Uint32 flags = SDL_GetWindowFlags(state.window);
                    SDL_SetWindowFullscreen(state.window, !(flags & SDL_WINDOW_FULLSCREEN));
                }

                if (event.key.key == SDLK_RETURN && state.allowtyping) {
                    if (state.enteringname) {
                        if (strlen(state.inputText) != 0) {
                            player1.setName(state.inputText);
                        } else {
                            player1.setName("John Doe");
                        }

                        state.typing = false;
                        state.allowtyping = false;
                        state.enteringname = false;

                        SDL_StopTextInput(state.window);

                        for (auto& t : state.texts) {
                            if (t.texture) SDL_DestroyTexture(t.texture);
                        }
                        state.texts.clear();

                        std::cout << player1.getName() << std::endl;
                    }
                }

                if (event.key.key == SDLK_BACKSPACE && strlen(state.inputText) > 0 && state.allowtyping) {
                    if (state.enteringname) {
                        state.inputText[strlen(state.inputText) - 1] = '\0';
                        drawText(state, state.inputText[0] != '\0' ? state.inputText : "", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, 300, "user_input", true);
                    }
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);

        if (!state.enteringname) {
            for (const auto& w : walls) {
                SDL_SetRenderDrawColor(state.renderer, w.color.r, w.color.g, w.color.b, w.color.a);
                SDL_RenderFillRects(state.renderer, &w.size, 1);
            }

            const bool* keys = SDL_GetKeyboardState(NULL);
            player1.handleInput(keys, deltaTime);
            player1.update(deltaTime, walls);
            player1.render(state.renderer);
        }

        for (auto& t : state.texts) {
            if (t.texture) SDL_RenderTexture(state.renderer, t.texture, NULL, &t.dst);
        }

        SDL_RenderPresent(state.renderer);
    }

    cleanup(state);
    return 0;
}

void entername(SDLState& state) {
    state.enteringname = true;
    SDL_StartTextInput(state.window);
    state.allowtyping = true;
    
    startTypewriter(state, "Please Enter Your Name:", "fonts/arial.ttf", 32, { 255,255,255,255 }, 25, state.winheight / 2 - 100, 0.05f, true);
}

void cleanup(SDLState& state) {
    for (auto& t : state.texts) {
        if (t.texture) SDL_DestroyTexture(t.texture);
    }
    state.texts.clear();
    TTF_Quit();
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}