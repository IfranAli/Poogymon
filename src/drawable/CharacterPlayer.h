#ifndef GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_
#define GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_

#include "Character.h"
#include "../Tileset.h"
#include "../Objects/FrameConfig.h"

class CharacterPlayer final : public Character {
 public:
  explicit CharacterPlayer(FrameConfig frame_config);
  ~CharacterPlayer();

  void Tick() override;
  void Render() override;

  int mov_boundary_x = 4;
  int mov_boundary_y = 4;

  bool draw_map = true;

 private:
  void ResetBoundingBox();
  const char *texture_path = "Resource/red.png";
  Texture::Tileset texture;
  FrameConfig frame_config_;

  int x_max{};
  int y_max{};
  int x_min{};
  int y_min{};

  int player_min_x = {};
  int player_min_y = {};
  int player_max_x = {};
  int player_max_y = {};
};

namespace player
{
	extern CharacterPlayer* player;
	extern bool is_visible;
	extern bool draw_bounds;
}

#endif // GAME_SRC_DRAWABLE_CHARACTERPLAYER_H_
