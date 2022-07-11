#include "FrameConfig.h"
#include "../Renderer.h"

int FrameConfig::GetTileDimentions() const {
  return tile_dimentions_;
}
int FrameConfig::GetCols() const {
  return cols_;
}
int FrameConfig::GetRows() const {
  return rows_;
}
int FrameConfig::GetWidth() const {
  return width_;
}
int FrameConfig::GetHeight() const {
  return height_;
}
int FrameConfig::GetOffsetX() const {
  return offset_x_;
}
int FrameConfig::GetOffsetY() const {
  return offset_y_;
}

void FrameConfig::RecalculateWindowVariables() {
  RecalculateWindowVariables(DimentionType{
      .width = width_,
      .height = height_
  });
}

void FrameConfig::RecalculateWindowVariables(DimentionType dim) {
  offset_x_ = dim.width - width_;
  offset_y_ = 0;

  window_width = dim.width;
  window_height = dim.height;

  SDL_SetWindowSize(sdl::window, window_width, window_height);
}
