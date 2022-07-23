//
// Created by Ila Luvox on 23/7/2022.
//

#ifndef GAME_SRC_WORLD_DEFINITIONS_H_
#define GAME_SRC_WORLD_DEFINITIONS_H_

#include <string>
#include <sstream>
#include <fstream>
#include "Map.h"

namespace map {
  class Map;

  enum MapIndex : int {
    ACTIVE = 0,
    LEFT_CONNECTION = 1,
    RIGHT_CONNECTION = 2,
    TOP_CONNECTION = 3,
    BOTTOM_CONNECTION = 4,
  };

  enum Direction : int {
    UP,
    DOWN,
    LEFT,
    RIGHT,
  };

  extern Map *active_map;
}

#endif
