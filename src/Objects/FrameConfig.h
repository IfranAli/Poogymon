//
// Created by Ila Luvox on 3/7/2022.
//

#pragma once

struct DimensionType {
  int width = 0;
  int height = 0;
};

namespace config {
  static constexpr int TILE_PER_COLUMN = 16;
  static constexpr int TILE_PER_ROW = 10;
  static constexpr int TILE_DIMENSIONS = 32;
  static constexpr unsigned int MAP_ARRAY_SIZE = TILE_PER_ROW * TILE_PER_COLUMN;
}

class FrameConfig {
 public:
  [[nodiscard]] int GetTileDimension() const;
  [[nodiscard]] int GetCols() const;
  [[nodiscard]] int GetRows() const;
  [[nodiscard]] int GetWidth() const;
  [[nodiscard]] int GetHeight() const;
  [[nodiscard]] int GetOffsetX() const;
  [[nodiscard]] int GetOffsetY() const;

  void RecalculateWindowVariables(DimensionType);
  void RecalculateWindowVariables();

 private:
  const int cols_ = config::TILE_PER_COLUMN;
  const int rows_ = config::TILE_PER_ROW;

  int width_ = cols_ * config::TILE_DIMENSIONS;
  int height_ = rows_ * config::TILE_DIMENSIONS;
  int offset_x_ = 0;
  int offset_y_ = 0;
  int window_width_ = width_;
  int window_height_ = height_;
};
