//
// Created by Ila Luvox on 30/3/2022.
//

#include "Renderer.h"

namespace config {
int WINDOW_WIDTH = ::config::SCREEN_WIDTH;
int WINDOW_HEIGHT = ::config::SCREEN_HEIGHT;

int SCREEN_OFFSET_X = (WINDOW_WIDTH - ::config::SCREEN_WIDTH) / 2;
int SCREEN_OFFSET_Y = (WINDOW_HEIGHT - ::config::SCREEN_HEIGHT) / 2;

void RecalculateWindowVariables(int w, int h, int x, int y) {
  WINDOW_WIDTH = w;
  WINDOW_HEIGHT = h;

  WINDOW_WIDTH = (w / TILE_DIM) * TILE_DIM;

  if (x == 0 && y == 0) {
    SCREEN_OFFSET_X = (WINDOW_WIDTH - ::config::SCREEN_WIDTH) / 2;
    SCREEN_OFFSET_Y = (WINDOW_HEIGHT - ::config::SCREEN_HEIGHT) / 2;
  } else {
    SCREEN_OFFSET_X = x;
    SCREEN_OFFSET_Y = y;
  }

  SDL_SetWindowSize(sdl::window, WINDOW_WIDTH, WINDOW_HEIGHT);
}

} /* namespace config */


namespace render {
void DrawRectangle(SDL_Rect &rect, bool relative) {
  if (relative) {
    rect.x += config::SCREEN_OFFSET_X;
    rect.y += config::SCREEN_OFFSET_Y;
  }

  SDL_RenderDrawRect(sdl::renderer, &rect);
}
} /* namespace render */