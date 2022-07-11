//
// Created by Ila Luvox on 3/7/2022.
//

#pragma once

struct DimentionType {
  int width = 0;
  int height = 0;
};

class FrameConfig {
 public:
  int GetTileDimentions() const;
  int GetCols() const;
  int GetRows() const;
  int GetWidth() const;
  int GetHeight() const;
  int GetOffsetX() const;
  int GetOffsetY() const;

  void RecalculateWindowVariables(DimentionType);
  void RecalculateWindowVariables();

 private:
  const int tile_per_column_ = 16;
  const int tile_per_row_ = 10;
  const int tile_dimentions_ = 32;

  int cols_ = tile_per_column_;
  int rows_ = tile_per_row_;
  int width_ = cols_ * tile_dimentions_;
  int height_ = rows_ * tile_dimentions_;
  int offset_x_ = 0;
  int offset_y_ = 0;
  int window_width = width_;
  int window_height = height_;
};
