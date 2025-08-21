#include "assets.h"
#include <stdio.h>

int assets_init(Assets* a, SDL_Renderer* ren){
  a->ren = ren;
  int flags = IMG_INIT_PNG | IMG_INIT_JPG;
  if((IMG_Init(flags) & flags) != flags){
    fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
    return 0;
  }
  return 1;
}

SDL_Texture* assets_load_texture(Assets* a, const char* path){
  SDL_Surface* s = IMG_Load(path);
  if(!s){
    fprintf(stderr, "IMG_Load('%s'): %s\n", path, IMG_GetError());
    return NULL;
  }
  SDL_Texture* t = SDL_CreateTextureFromSurface(a->ren, s);
  SDL_FreeSurface(s);
  return t;
}

SDL_Texture* assets_make_fallback(Assets* a, int w, int h){
  Uint32 fmt = SDL_PIXELFORMAT_RGBA8888;
  SDL_Surface* s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, fmt);
  SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, 220, 60, 60, 255));
  SDL_Rect r = {w/8, h/8, w*3/4, h*3/4};
  SDL_FillRect(s, &r, SDL_MapRGBA(s->format, 255, 220, 40, 255));
  SDL_Texture* t = SDL_CreateTextureFromSurface(a->ren, s);
  SDL_FreeSurface(s);
  return t;
}
