#include "CharacterPlayer.h"
#include "../Renderer.h"

namespace player {
  bool is_visible = false;
  bool draw_bounds = true;

  float BASE_MOVEMENT_SPEED = 3.5F;
  float BASE_ANIMATION_SPEED = BASE_MOVEMENT_SPEED * 4.0F;
  float RUNNING_SPEED = 2.0F;
  float RUN_ANIMATION_SPEED = BASE_ANIMATION_SPEED * RUNNING_SPEED;
  float RUN_MOVEMENT_SPEED = BASE_MOVEMENT_SPEED * RUNNING_SPEED;

  const int PLAYER_TEXTURE_WIDTH = 32;
  const int PLAYER_TEXTURE_HEIGHT = 48;

}

CharacterPlayer::~CharacterPlayer() = default;

CharacterPlayer::CharacterPlayer(FrameConfig &frame_config, map::Map *map) :
    map_(map),
    frame_config_(frame_config) {

  Texture::LoadFromFile(*sdl::renderer,
                        texture_,
                        texture_path_,
                        player::PLAYER_TEXTURE_WIDTH,
                        player::PLAYER_TEXTURE_HEIGHT);
  ResetBoundingBox();
}

void CharacterPlayer::ResetBoundingBox() {
  mov_boundary_x_ = default_mov_boundary_;
  mov_boundary_y_ = default_mov_boundary_;

  auto config = frame_config_;
  x_min_ = (config.GetCols() / 2) - (mov_boundary_x_ / 2);
  y_min_ = (config.GetRows() / 2) - (mov_boundary_y_ / 2);

  x_max_ = x_min_ + mov_boundary_x_;
  y_max_ = y_min_ + mov_boundary_y_;

  SetPlayerPosition(x_min_ + 1, y_min_ + 1);
}

void CharacterPlayer::CalculateBoundingBox() {
  y_min_ = (frame_config_.GetRows() / 2) - (mov_boundary_y_ / 2);
  y_max_ = y_min_ + mov_boundary_y_;
  x_min_ = (frame_config_.GetCols() / 2) - (mov_boundary_x_ / 2);
  x_max_ = x_min_ + mov_boundary_x_;

  if (map_->y_ == -(config::TILE_PER_ROW)) {
    y_min_ = -(config::TILE_PER_ROW);
  }

  if (map_->y_ == config::TILE_PER_ROW) {
    y_max_ = config::TILE_PER_ROW;
  }

  if (map_->x_ == -(config::TILE_PER_COLUMN)) {
    x_min_ = -(config::TILE_PER_COLUMN);
  }

  if (map_->x_ == config::TILE_PER_COLUMN) {
    x_max_ = config::TILE_PER_COLUMN;
  }
}

void CharacterPlayer::Tick() {
  // Step any animations
  if (animation_.currently_playing) {
    animation_.Tick();

    // Stop animations if a map transition has completed
    if (!movement_.moving) {
      animation_.Stop_animation();
    }
  }

  // Do not process movement_ input until map has completed transition animation
  if (movement_.moving) {
    movement_.Step();

    // Has movement_ ceased
    if (!movement_.moving) {
      // Stop all animations
      if (animation_.currently_playing)
        animation_.Stop_animation();

      // This is where world map can be gotten.
//			int player_world_x = movement_.x + map::active_map->GetX();
//			int player_world_y = movement_.y + map::active_map->GetY();
      //printf("Player at (%direction, %direction)\n", player_world_x, player_world_y);
//			map::active_map->GetActiveMapData().SetTile(player_world_x, player_world_y, 11);
    } else {
      return;
    }

    //return;
  }

  // Thus far:
  // Map is not moving
  // Player is not moving
  // Animation is not playing


  if (velocity_x_ == 0 && velocity_y_ == 0) {
    return;
  }

  if (is_running_) {
    animation_.speed = player::RUN_ANIMATION_SPEED;
    movement_.speed = player::RUN_MOVEMENT_SPEED;
  } else {
    animation_.speed = player::BASE_ANIMATION_SPEED;
    movement_.speed = player::BASE_MOVEMENT_SPEED;
  }

  map::Direction direction =
      (velocity_x_ == 0) ? (velocity_y_ == 1 ? map::DOWN : map::UP) : (velocity_x_ == 1 ? map::RIGHT : map::LEFT);
  CalculateBoundingBox();
  MovePlayer(direction);

  x_pos_min_ = x_min_;
  x_pos_max_ = x_max_;
  y_pos_min_ = y_min_;
  y_pos_max_ = y_max_;
}

void CharacterPlayer::MovePlayer(const map::Direction &direction) {
  // Doesn't work in editor mode.
  if (movement_.moving) {
    return;
  }
  int potential_x = map_->GetX() + movement_.x + velocity_x_;
  int potential_y = map_->GetY() + movement_.y + velocity_y_;

  auto x_pos = static_cast<int>(movement_.x);
  auto y_pos = static_cast<int>(movement_.y);

  switch (direction) {
    case map::RIGHT :
      if (map_->x_ == config::TILE_PER_COLUMN && (movement_.x + velocity_x_) >= x_max_) {
        return;
      }

      if (x_pos >= x_max_) {
        movement_.MoveSmooth(1, 0, true);
      } else {
        movement_.MoveSmooth(1, 0);
      }

      animation_.Play_animation(8);
      break;
    case map::LEFT:
      if (map_->x_ == -(config::TILE_PER_COLUMN) && potential_x < x_min_) {
        return;
      }

      if (x_pos <= x_min_) {
        movement_.MoveSmooth(-1, 0, true);
      } else {
        movement_.MoveSmooth(-1, 0);
      }

      animation_.Play_animation(4);
      break;
    case map::DOWN:
      if (map_->y_ == config::TILE_PER_ROW && ((movement_.y + velocity_y_) >= y_max_)) {
        return;
      }

      if (y_pos >= y_max_) {
        movement_.MoveSmooth(0, 1, true);
      } else {
        movement_.MoveSmooth(0, 1);
      }

      animation_.Play_animation(0);
      break;
    case map::UP:
      if (map_->y_ == -(config::TILE_PER_ROW) && potential_y < y_min_) {
        return;
      }

      if (y_pos <= y_min_) {
        movement_.MoveSmooth(0, -1, true);
      } else {
        movement_.MoveSmooth(0, -1);
      }

      animation_.Play_animation(12);
      break;
  }

  printf("Position: X: %d, Y: %d\n", potential_x, potential_y);
}

void CharacterPlayer::Render() {
  // Render player
  auto offset_x = frame_config_.GetOffsetX();
  auto offset_y = frame_config_.GetOffsetY();

  const auto TILE_DIM = static_cast<float>(this->frame_config_.GetTileDimension());
  const float PLAYER_X = static_cast<float>(offset_x) + (movement_.x * TILE_DIM);
  const float PLAYER_Y = static_cast<float>(offset_y) + (movement_.y * TILE_DIM);
  const float OFFSET_Y = 20.0F;

  Texture::Render(
      *sdl::renderer,
      texture_,
      animation_.current_frame,
      static_cast<int>(PLAYER_X),
      static_cast<int>(PLAYER_Y - OFFSET_Y)
  );

  // Player box
  if (player::draw_bounds) {
    auto player_box = SDL_Rect{
        (x_pos_min_ * frame_config_.GetTileDimension()) + offset_x,
        (y_pos_min_ * frame_config_.GetTileDimension()) + offset_y,
        ((x_pos_max_ - x_pos_min_) * frame_config_.GetTileDimension()) + frame_config_.GetTileDimension(),
        ((y_pos_max_ - y_pos_min_) * frame_config_.GetTileDimension()) + frame_config_.GetTileDimension()
    };
    auto player_character_box = SDL_Rect{
        static_cast<int>(PLAYER_X),
        static_cast<int>(PLAYER_Y),
        frame_config_.GetTileDimension(),
        frame_config_.GetTileDimension(),
    };
    auto map_box = SDL_Rect{
        offset_x,
        offset_y,
        frame_config_.GetWidth(),
        frame_config_.GetHeight()
    };

    SDL_SetRenderDrawColor(sdl::renderer, 0xFF, 0x00, 0x00, 0xFF);

    SDL_RenderDrawRect(sdl::renderer, &player_box);
    SDL_RenderDrawRect(sdl::renderer, &player_character_box);
    SDL_RenderDrawRect(sdl::renderer, &map_box);
  }
}
Movement &CharacterPlayer::GetMovementObject() {
  return movement_;
}

void CharacterPlayer::SetMoveBoundary(int x, int y) {
  mov_boundary_x_ = x;
  mov_boundary_y_ = y;
  CalculateBoundingBox();
}
void CharacterPlayer::SetPlayerPosition(int x, int y) {
  Character::movement_.x = static_cast<float>(x);
  Character::movement_.y = static_cast<float>(y);
}
