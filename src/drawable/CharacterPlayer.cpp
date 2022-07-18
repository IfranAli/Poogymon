#include "CharacterPlayer.h"
#include "../Renderer.h"

namespace player {
  bool is_visible = false;
  bool draw_bounds = true;

  constexpr float BASE_ANIMATION_SPEED = 11;
  constexpr float BASE_MOVEMENT_SPEED = 3;
  constexpr float RUNNING_SPEED = 1.8;
  constexpr float RUN_ANIMATION_SPEED = BASE_ANIMATION_SPEED * RUNNING_SPEED;
  constexpr float RUN_MOVEMENT_SPEED = BASE_MOVEMENT_SPEED * RUNNING_SPEED;
}

CharacterPlayer::~CharacterPlayer() = default;

CharacterPlayer::CharacterPlayer(FrameConfig &frame_config, map::Map *map) :
    map_(map),
    frame_config_(frame_config) {

  Texture::LoadFromFile(*sdl::renderer, texture_, texture_path_, 32, 48);
  ResetBoundingBox();
}

void CharacterPlayer::ResetBoundingBox() {
  auto config = frame_config_;

  x_min_ = (config.GetCols() / 2) - (mov_boundary_x_ / 2);
  y_min_ = (config.GetRows() / 2) - (mov_boundary_y_ / 2);

  x_max_ = x_min_ + mov_boundary_x_;
  y_max_ = y_min_ + mov_boundary_y_;

  /* Place character inside bounds */
  Character::movement.x = static_cast<float>(x_min_);
  Character::movement.y = static_cast<float>(y_min_);

  printf("Player - bounding box update: W:%lu, H:%lu\n", mov_boundary_x_, mov_boundary_y_);
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
  if (animation.currently_playing) {
    animation.Tick();

    // Stop animations if a map transition has completed
    if (!movement.moving && !map::moving)
      animation.Stop_animation();
  }

  // Do not process movement input until map has completed transition animation
  if (movement.moving) {
    movement.Step();

    // Has movement ceased
    if (!movement.moving) {
      // Stop all animations
      if (animation.currently_playing)
        animation.Stop_animation();

      // This is where world map can be gotten.
//			int player_world_x = movement.x + map::active_map->GetX();
//			int player_world_y = movement.y + map::active_map->GetY();
      //printf("Player at (%direction, %direction)\n", player_world_x, player_world_y);
//			map::active_map->active->SetTile(player_world_x, player_world_y, 11);
    } else {
      return;
    }

    //return;
  }

  // Thus far:
  // Map is not moving
  // Player is not moving
  // Animation is not playing


  if (mVelocityX == 0 && mVelocityY == 0) {
    return;
  }

  map::Direction direction =
      (mVelocityX == 0) ? (mVelocityY == 1 ? map::DOWN : map::UP) : (mVelocityX == 1 ? map::RIGHT : map::LEFT);

  if (is_running) {
    animation.speed = player::RUN_ANIMATION_SPEED;
    movement.speed = player::RUN_MOVEMENT_SPEED;
  } else {
    animation.speed = player::BASE_ANIMATION_SPEED;
    movement.speed = player::BASE_MOVEMENT_SPEED;
  }

  int potential_x = map::active_map->GetX() + movement.x + mVelocityX;
  int potential_y = map::active_map->GetY() + movement.y + mVelocityY;

  auto x_pos = static_cast<int>(movement.x);
  auto y_pos = static_cast<int>(movement.y);

  CalculateBoundingBox();

  if (map_->y_ == -(config::TILE_PER_ROW) && potential_y < y_min_) {
    return;
  }

  if (map_->y_ == config::TILE_PER_ROW && ((movement.y + mVelocityY) >= y_max_)) {
    return;
  }

  if (map_->x_ == -(config::TILE_PER_COLUMN) && potential_x < x_min_) {
    return;
  }

  if (map_->x_ == config::TILE_PER_COLUMN && (movement.x + mVelocityX) >= x_max_) {
    return;
  }

  switch (direction) {
    case map::RIGHT :
      if (x_pos >= x_max_) {
        movement.MoveSmooth(1, 0, true);
      } else {
        movement.MoveSmooth(1, 0);
      }

      animation.Play_animation(8);
      break;
    case map::LEFT:
      if (x_pos <= x_min_) {
        movement.MoveSmooth(-1, 0, true);
      } else {
        movement.MoveSmooth(-1, 0);
      }

      animation.Play_animation(4);
      break;
    case map::DOWN:
      if (y_pos >= y_max_) {
        movement.MoveSmooth(0, 1, true);
      } else {
        movement.MoveSmooth(0, 1);
      }

      animation.Play_animation(0);
      break;
    case map::UP:
      if (y_pos <= y_min_) {
        movement.MoveSmooth(0, -1, true);
      } else {
        movement.MoveSmooth(0, -1);
      }

      animation.Play_animation(12);
      break;
  }

  map::direction_moving = direction;

  x_pos_min_ = x_min_;
  x_pos_max_ = x_max_;
  y_pos_min_ = y_min_;
  y_pos_max_ = y_max_;

  printf("Position: X: %d, Y: %d\n", potential_x, potential_y);
}

void CharacterPlayer::Render() {
  // Render player
  auto offset_x = frame_config_.GetOffsetX();
  auto offset_y = frame_config_.GetOffsetY();

  const auto TILE_DIM = static_cast<float>(this->frame_config_.GetTileDimension());
  const float PLAYER_X = static_cast<float>(offset_x) + (movement.x * TILE_DIM);
  const float PLAYER_Y = static_cast<float>(offset_y) + (movement.y * TILE_DIM);
  const float OFFSET_Y = 20.0F;

  Texture::Render(
      *sdl::renderer,
      texture_,
      animation.current_frame,
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
