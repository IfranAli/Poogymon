#include "FrameConfig.h"
#include "../Renderer.h"

int FrameConfig::GetTileDimension() const {
  return config::TILE_DIMENSIONS;
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
  RecalculateWindowVariables(DimensionType{
      .width = width_,
      .height = height_
  });
}

void FrameConfig::RecalculateWindowVariables(DimensionType dim) {
  offset_x_ = dim.width - width_;
  offset_y_ = 0;

  window_width_ = dim.width;
  window_height_ = dim.height;

  SDL_SetWindowSize(sdl::window, window_width_, window_height_);
}

