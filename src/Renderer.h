#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "interfaces/Drawable.h"
#include "Objects/FrameConfig.h"

namespace sdl {
  extern SDL_Window *window;
  extern SDL_Renderer *renderer;
  extern float deltatime;
  extern FrameConfig *g_frame_config;
} /* sdl */