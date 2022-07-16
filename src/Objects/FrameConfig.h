//
// Created by Ila Luvox on 3/7/2022.
//

#pragma once

struct DimensionType {
  int width = 0;
  int height = 0;
};

class FrameConfig {
 public:
  int GetTileDimension() const;
  int GetCols() const;
  int GetRows() const;
  int GetWidth() const;
  int GetHeight() const;
  int GetOffsetX() const;
  int GetOffsetY() const;

  void RecalculateWindowVariables(DimensionType);
  void RecalculateWindowVariables();

 private:
  const int tile_per_column_ = 16;
  const int tile_per_row_ = 10;
  const int tile_dimensions_ = 32;

  int cols_ = tile_per_column_;
  int rows_ = tile_per_row_;
  int width_ = cols_ * tile_dimensions_;
  int height_ = rows_ * tile_dimensions_;
  int offset_x_ = 0;
  int offset_y_ = 0;
  int window_width = width_;
  int window_height = height_;
};
