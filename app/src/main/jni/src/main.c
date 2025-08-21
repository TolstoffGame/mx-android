#include "engine.h"
#include "assets.h"
#include <SDL.h>
#include <math.h>
#include <stdio.h>

typedef struct { float x,y,w,h; float vx,vy; SDL_Texture* tex; } Sprite;

static void draw_tex(SDL_Renderer* r, SDL_Texture* t, int x,int y,int w,int h){
  SDL_Rect dst = {x,y,w,h};
  SDL_RenderCopy(r, t, NULL, &dst);
}

// чисто-C обработчик точки (тач/мышь)
static void handle_point(float px, float py, int press,
                         int screen_w, int screen_h,
                         float joy_cx, float joy_cy, float joy_r,
                         int* p_joy_active, float* p_knob_x, float* p_knob_y,
                         float* p_vx, float* p_vy)
{
  if(press && (*p_joy_active || px < screen_w*0.5f)){
    *p_joy_active = 1;
    float dx = px - joy_cx, dy = py - joy_cy;
    float len = sqrtf(dx*dx + dy*dy);
    if(len > 1e-3f){
      float k = fminf(len, joy_r)/len;
      *p_knob_x = joy_cx + dx*k;
      *p_knob_y = joy_cy + dy*k;
      *p_vx = dx / (len > 1.f ? len : 1.f);
      *p_vy = dy / (len > 1.f ? len : 1.f);
    } else {
      *p_knob_x = joy_cx; *p_knob_y = joy_cy;
      *p_vx = 0; *p_vy = 0;
    }
  } else if(!press){
    *p_joy_active = 0;
    *p_knob_x = joy_cx; *p_knob_y = joy_cy;
    *p_vx = 0; *p_vy = 0;
  }
}

int main(){
  Engine E;
  if(!eng_init(&E, "MX Engine — sprite + touch (C99)", 1280, 720)) return 1;

  Assets A;
  assets_init(&A, E.ren);

  SDL_Texture* hero = assets_load_texture(&A, "/sdcard/Projects/mx-engine/assets/hero.png");
  if(!hero) hero = assets_make_fallback(&A, 96, 96);

  Sprite player = { E.width*0.5f-48, E.height*0.5f-48, 96, 96, 0, 0, hero };

  // виртуальный стик
  const float joy_cx = E.width * 0.18f;
  const float joy_cy = E.height * 0.82f;
  const float joy_r  = 140.0f;
  int   joy_active = 0;
  float knob_x = joy_cx, knob_y = joy_cy;

  SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

  Uint32 prev = SDL_GetTicks();
  while(E.running){
    SDL_Event ev;
    while(SDL_PollEvent(&ev)){
      if(ev.type == SDL_QUIT) E.running = 0;
      if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) E.running = 0;

      if(ev.type==SDL_FINGERDOWN || ev.type==SDL_FINGERMOTION){
        float px = ev.tfinger.x * E.width;
        float py = ev.tfinger.y * E.height;
        handle_point(px, py, 1, E.width, E.height, joy_cx, joy_cy, joy_r,
                     &joy_active, &knob_x, &knob_y, &player.vx, &player.vy);
      }
      if(ev.type==SDL_FINGERUP){
        handle_point(0, 0, 0, E.width, E.height, joy_cx, joy_cy, joy_r,
                     &joy_active, &knob_x, &knob_y, &player.vx, &player.vy);
      }
      if(ev.type==SDL_MOUSEBUTTONDOWN && ev.button.button==SDL_BUTTON_LEFT){
        handle_point((float)ev.button.x, (float)ev.button.y, 1, E.width, E.height, joy_cx, joy_cy, joy_r,
                     &joy_active, &knob_x, &knob_y, &player.vx, &player.vy);
      }
      if(ev.type==SDL_MOUSEMOTION && joy_active){
        handle_point((float)ev.motion.x, (float)ev.motion.y, 1, E.width, E.height, joy_cx, joy_cy, joy_r,
                     &joy_active, &knob_x, &knob_y, &player.vx, &player.vy);
      }
      if(ev.type==SDL_MOUSEBUTTONUP && ev.button.button==SDL_BUTTON_LEFT){
        handle_point(0, 0, 0, E.width, E.height, joy_cx, joy_cy, joy_r,
                     &joy_active, &knob_x, &knob_y, &player.vx, &player.vy);
      }
    }

    Uint32 now = SDL_GetTicks();
    float dt = (now - prev)/1000.0f; if(dt>0.033f) dt=0.033f; prev = now;

    float speed = 300.f;
    player.x += player.vx * speed * dt;
    player.y += player.vy * speed * dt;
    if(player.x<0) player.x=0;
    if(player.y<0) player.y=0;
    if(player.x+player.w>E.width)  player.x=E.width-player.w;
    if(player.y+player.h>E.height) player.y=E.height-player.h;

    eng_begin(&E);
      SDL_SetRenderDrawColor(E.ren, 18,18,22,255);
      SDL_RenderClear(E.ren);
      // игрок-спрайт
      draw_tex(E.ren, player.tex, (int)player.x, (int)player.y, (int)player.w, (int)player.h);
      // стик
      SDL_SetRenderDrawColor(E.ren, 60,60,66,255);
      SDL_Rect ring = {(int)(joy_cx-joy_r),(int)(joy_cy-joy_r),(int)(joy_r*2),(int)(joy_r*2)};
      SDL_RenderDrawRect(E.ren,&ring);
      SDL_SetRenderDrawColor(E.ren, 180,180,190,255);
      SDL_Rect knob={(int)(knob_x-28),(int)(knob_y-28),56,56};
      SDL_RenderFillRect(E.ren,&knob);
    eng_end(&E);
  }

  eng_shutdown(&E);
  return 0;
}
