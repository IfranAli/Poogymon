#ifndef GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_
#define GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_

#include "Character.h"
#include "../Tileset.h"
#include "../Objects/FrameConfig.h"
#include "../Map.h"

class CharacterPlayer final : public Character {
 public:
  CharacterPlayer(FrameConfig &frame_config, map::Map *map);
  ~CharacterPlayer();

  void Tick() override;
  void Render() override;

 private:
  map::Map *map_;

  void ResetBoundingBox();
  void CalculateBoundingBox();

  const char *texture_path_ = "Resource/red.png";
  Texture::Tileset texture_;
  FrameConfig &frame_config_;

  int mov_boundary_x_ = 2;
  int mov_boundary_y_ = 2;

  int x_max_{};
  int y_max_{};
  int x_min_{};
  int y_min_{};

  int x_pos_min_ = 0;
  int x_pos_max_ = 0;
  int y_pos_min_ = 0;
  int y_pos_max_ = 0;
};

namespace player {
  extern CharacterPlayer *player;
  extern bool is_visible;
  extern bool draw_bounds;
}

#endif // GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_
