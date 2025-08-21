#include "engine.h"
#include <stdio.h>

int eng_init(Engine* e, const char* title, int w, int h) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return 0;
  }
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  e->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            w, h, SDL_WINDOW_SHOWN);
  if (!e->win) {
    fprintf(stderr, "CreateWindow error: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }
  e->ren = SDL_CreateRenderer(e->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!e->ren) {
    e->ren = SDL_CreateRenderer(e->win, -1, 0); // софт-фоллбек
  }
  if (!e->ren) {
    fprintf(stderr, "CreateRenderer error: %s\n", SDL_GetError());
    SDL_DestroyWindow(e->win);
    SDL_Quit();
    return 0;
  }
  e->running = 1;
  e->width = w;
  e->height = h;
  return 1;
}

void eng_begin(Engine* e) {
  SDL_SetRenderDrawColor(e->ren, 0, 0, 0, 255);
  SDL_RenderClear(e->ren);
}

void eng_end(Engine* e) {
  SDL_RenderPresent(e->ren);
}

void eng_shutdown(Engine* e) {
  if (e->ren) SDL_DestroyRenderer(e->ren);
  if (e->win) SDL_DestroyWindow(e->win);
  SDL_Quit();
}
