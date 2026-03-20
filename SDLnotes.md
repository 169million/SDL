
---

<center><ins>Basic SDL Window</ins></center>
<p></p>

```cpp
#include <SDL.h>
#include <SDL_main.h>

int main(int argc, char* argv[]){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        return 1;
    }
    int winwidth = 600;
    int winheight = 400;
    SDL_Window* window = SDL_CreateWindow("Basic Window", winwidth, winheight, nullptr);

    bool stop = false;
    while (!stop){
        SDL_Event event{0};
        while SDL_PollEvent(&event){
            switch (event.type){
            case SDL_EVENT_QUIT:
                stop = true;
                break;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
```

---
<center><ins>DeltaTime</ins></center>

**<center>BEFORE MAINLOOP</center>**
```cpp
Uint64 lastTime = SDL_GetPerformanceCounter();
float deltaTime = 0.0f;
```

**<center>AT TOP OF MAINLOOP</center>**

```cpp
Uint64 now = SDL_GetPerformanceCounter();
deltaTime = (float)(now - lastTime) / (float)SDL_GetPerformanceFrequency();
lastTime = now;
deltaTime = SDL_min(deltaTime, 0.05f);
```


[NOTE - Using Deltatime w/ sdl causes things like gravity to be px/sec (values need to be high)]

---
