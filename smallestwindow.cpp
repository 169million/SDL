#include <SDL3/SDL.h>
int main(){SDL_Init(SDL_INIT_VIDEO);SDL_Window*A=SDL_CreateWindow("",210,0,0);SDL_Event B;while(SDL_WaitEvent(&B)&&B.type!=SDL_EVENT_QUIT);SDL_DestroyWindow(A);SDL_Quit();}