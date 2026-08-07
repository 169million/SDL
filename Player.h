#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "dialogue.h"

enum class AnimState {
    IDLE,
    RUN,
    JUMP
};

struct Animation {
    std::vector<SDL_Texture*> frames;
    float frameDuration = 0.15f;
    bool loop = true;
};

class Player {
private:
    SDL_FRect rect = {};
    SDL_FRect oldRect = {};

    std::unordered_map<AnimState, Animation> animations;
    AnimState currentState = AnimState::IDLE;
    int currentFrame = 0;
    float animTimer = 0.0f;

    float velocity = 0.0f;
    float speed = 200.0f;
    float gravity = 750.0f;
    bool grounded = false;
    char name[50] = {};

public:
    Player();
    ~Player();

    void addAnimation(SDL_Renderer* renderer, AnimState state, const std::vector<std::string>& framePaths, float frameDuration = 0.15f, bool loop = true);
    void setAnimation(AnimState newState);
    void handleInput(const bool* keys, float deltaTime);
    void update(float deltaTime, const std::vector<CollisionObject>& walls);
    void render(SDL_Renderer* renderer);
    void setName(const char* newName);
    const char* getName() const;
};