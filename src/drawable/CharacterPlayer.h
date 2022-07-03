#ifndef GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_
#define GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_

#include "Character.h"
#include "../Tileset.h"
#include "../Objects/FrameConfig.h"
#include "../Map.h"

class CharacterPlayer final : public Character {
 public:
  CharacterPlayer(FrameConfig frame_config, map::Map *map);
  ~CharacterPlayer();

  void Tick() override;
  void Render() override;

  int mov_boundary_x = 2;
  int mov_boundary_y = 2;

  bool draw_map = true;

 private:
  map::Map *map_;
  void ResetBoundingBox();
  const char *texture_path = "Resource/red.png";
  Texture::Tileset texture;
  FrameConfig frame_config_;

  int x_max{};
  int y_max{};
  int x_min{};
  int y_min{};

  int x_pos_min = 0;
  int x_pos_max = 0;
  int y_pos_min = 0;
  int y_pos_max = 0;
};

namespace player {
  extern CharacterPlayer *player;
  extern bool is_visible;
  extern bool draw_bounds;
}

#endif // GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_
