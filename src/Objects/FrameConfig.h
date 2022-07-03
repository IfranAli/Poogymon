//
// Created by Ila Luvox on 3/7/2022.
//

#ifndef GAME_SRC_OBJECTS_FRAMECONFIG_H_
#define GAME_SRC_OBJECTS_FRAMECONFIG_H_

constexpr int TILE_PER_COLUMN = 15;
constexpr int TILE_PER_ROW = 10;
constexpr int TILE_DIMENTIONS = 32;

struct FrameConfig {
  int tile_dimentions = TILE_DIMENTIONS;
  int cols = TILE_PER_COLUMN;
  int rows = TILE_PER_ROW;
  int width = cols * tile_dimentions;
  int height = rows * tile_dimentions;
  int offset_x = 0;
  int offset_y = 0;
};

#endif //GAME_SRC_OBJECTS_FRAMECONFIG_H_
