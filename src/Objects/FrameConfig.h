//
// Created by Ila Luvox on 3/7/2022.
//

#ifndef GAME_SRC_OBJECTS_FRAMECONFIG_H_
#define GAME_SRC_OBJECTS_FRAMECONFIG_H_

namespace internal {
  constexpr int TILE_PER_COLUMN = 16;
  constexpr int TILE_PER_ROW = 10;
  constexpr int TILE_DIMENTIONS = 32;
}

struct FrameConfig {
  int tile_dimentions = internal::TILE_DIMENTIONS;
  int cols = internal::TILE_PER_COLUMN;
  int rows = internal::TILE_PER_ROW;
  int width = cols * tile_dimentions;
  int height = rows * tile_dimentions;
  int offset_x = 0;
  int offset_y = 0;
};

#endif //GAME_SRC_OBJECTS_FRAMECONFIG_H_
