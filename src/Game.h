//
// Created by Ila Luvox on 3/7/2022.
//

#ifndef GAME_SRC_GAME_H_
#define GAME_SRC_GAME_H_

#include "Objects/FrameConfig.h"
#include "Map.h"
#include "InputContext.h"
#include "drawable/CharacterPlayer.h"
#include "RenderManager.h"
class Game {
 public:

  Game(FrameConfig &frame_config);
  virtual ~Game();

  void Start();

 private:
  bool quit_ = false;

  FrameConfig &frame_config_;
  map::Map map_;
  RenderManager render_manager_;
  input::InputContext input_context_;
  CharacterPlayer player_;

  void GameLoop();
};

#endif //GAME_SRC_GAME_H_
