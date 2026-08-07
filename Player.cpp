#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <cstring>
#include <iostream>

Player::Player() {}

Player::~Player() {
    for (auto& pair : animations) {
        for (SDL_Texture* tex : pair.second.frames) {
            if (tex) SDL_DestroyTexture(tex);
        }
        pair.second.frames.clear();
    }
    animations.clear();
}

void Player::addAnimation(SDL_Renderer* renderer, AnimState state, const std::vector<std::string>& framePaths, float frameDuration, bool loop) {
    Animation anim;
    anim.frameDuration = frameDuration;
    anim.loop = loop;

    for (const auto& path : framePaths) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) continue;

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        if (tex) {
            anim.frames.push_back(tex);
        }

        if (rect.w == 0 && rect.h == 0) {
            rect = { 350.0f, 250.0f, (float)surface->w, (float)surface->h };
            oldRect = rect;
        }

        SDL_DestroySurface(surface);
    }

    animations[state] = anim;
}

void Player::setAnimation(AnimState newState) {
    if (currentState == newState) return;
    currentState = newState;
    currentFrame = 0;
    animTimer = 0.0f;
}

void Player::handleInput(const bool* keys, float deltaTime) {
    oldRect = rect;

    int movex = 0;
    if (keys[SDL_SCANCODE_A]) movex = -1;
    if (keys[SDL_SCANCODE_D]) movex = 1;

    rect.x += movex * speed * deltaTime;

    if ((keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_SPACE]) && grounded) {
        velocity = -500.0f;
        grounded = false;
    }

    if (!grounded) {
        setAnimation(AnimState::JUMP);
    } else if (movex != 0) {
        setAnimation(AnimState::RUN);
    } else {
        setAnimation(AnimState::IDLE);
    }
}

void Player::update(float deltaTime, const std::vector<CollisionObject>& walls) {
    auto it = animations.find(currentState);
    if (it != animations.end() && !it->second.frames.empty()) {
        Animation& anim = it->second;
        animTimer += deltaTime;

        if (animTimer >= anim.frameDuration) {
            animTimer -= anim.frameDuration;
            if (currentFrame + 1 < (int)anim.frames.size()) {
                currentFrame++;
            } else if (anim.loop) {
                currentFrame = 0;
            }
        }
    }

    velocity += gravity * deltaTime;
    rect.y += velocity * deltaTime;

    grounded = false;
    for (const auto& w : walls) {
        auto& ws = w.size;
        if (ws.w == 0 || ws.h == 0) continue;

        bool overlapX = rect.x < ws.x + ws.w && rect.x + rect.w > ws.x;
        bool overlapY = rect.y < ws.y + ws.h && rect.y + rect.h > ws.y;
        bool oldOverlapX = oldRect.x < ws.x + ws.w && oldRect.x + rect.w > ws.x;
        bool oldOverlapY = oldRect.y < ws.y + ws.h && oldRect.y + rect.h > ws.y;

        if (overlapX && overlapY) {
            if (!oldOverlapX) {
                if (rect.x < ws.x) rect.x = ws.x - rect.w;
                else rect.x = ws.x + ws.w;
            }
            if (!oldOverlapY) {
                if (velocity > 0) {
                    rect.y = ws.y - rect.h;
                    grounded = true;
                } else if (velocity < 0) {
                    rect.y = ws.y + ws.h;
                }
                velocity = 0;
            }
        }
    }
}

void Player::render(SDL_Renderer* renderer) {
    auto it = animations.find(currentState);
    if (it != animations.end() && !it->second.frames.empty()) {
        SDL_Texture* activeTex = it->second.frames[currentFrame];
        if (activeTex) {
            SDL_RenderTexture(renderer, activeTex, NULL, &rect);
        }
    }
}

void Player::setName(const char* newName) {
    if (newName) {
        strncpy(name, newName, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }
}

const char* Player::getName() const {
    return name;
}