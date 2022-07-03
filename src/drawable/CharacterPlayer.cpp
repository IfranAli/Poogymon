#include "CharacterPlayer.h"
#include "../Renderer.h"
#include "../Map.h"

#include <iostream>

namespace player {
  bool is_visible{false};
  bool draw_bounds{true};
}

void CharacterPlayer::ResetBoundingBox() {
  auto config = frame_config_;

  x_min = (config.cols / 2) - (mov_boundary_x / 2);
  y_min = (config.rows / 2) - (mov_boundary_y / 2);

  x_max = x_min + mov_boundary_x;
  y_max = y_min + mov_boundary_y;

  /* Place character inside bounds */
  Character::movement.x = static_cast<float>(x_min);
  Character::movement.y = static_cast<float>(y_min);

  printf("Player - bounding box update: W:%lu, H:%lu\n", mov_boundary_x, mov_boundary_y);
}

CharacterPlayer::~CharacterPlayer() = default;

CharacterPlayer::CharacterPlayer(FrameConfig frame_config, map::Map *map) :
    map_(map),
    frame_config_(frame_config) {

  Texture::LoadFromFile(*sdl::renderer, texture, texture_path, 32, 48);
  ResetBoundingBox();
}

constexpr float base_animation_speed = 11;
constexpr float base_movement_speed = 3;
constexpr float running_speed = 1.8;
constexpr float run_animation_speed = base_animation_speed * running_speed;
constexpr float run_movement_speed = base_movement_speed * running_speed;

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
    animation.speed = run_animation_speed;
    movement.speed = run_movement_speed;
  } else {
    animation.speed = base_animation_speed;
    movement.speed = base_movement_speed;
  }

  map::active_map->transition_speed_ = movement.speed;

  int potential_x = map::active_map->GetX() + movement.x + mVelocityX;
  int potential_y = map::active_map->GetY() + movement.y + mVelocityY;

  printf("X: %direction, Y: %direction\n", potential_x, potential_y);

  auto canMoveRight = potential_x > 0 || potential_x != 20;
  auto canMoveLeft = potential_x > 0;
  auto canMoveUp = potential_y > 0;
  auto canMoveDown = potential_y > 0 || potential_y != 20;

  if (potential_x < 0 || potential_x > 20) {
    return;
  }

  if (potential_y < 0 || potential_y > 20) {
    return;
  }

  auto cols = 15;
  auto rows = 10;

  auto x_pos = static_cast<int>(movement.x);
  auto y_pos = static_cast<int>(movement.y);

  x_pos_min = x_min;
  x_pos_max = x_max;
  y_pos_min = y_min;
  y_pos_max = y_max;

  if (x_pos <= x_min && (map_->x == 0.0F)) {
    x_pos_min = 0;
    if (potential_x < 0) {
      return;
    }
  } else if (x_pos >= x_max && (map_->x == static_cast<float>(this->map_->x_max_ - this->frame_config_.cols))) {
    x_pos_max = frame_config_.cols - 1;
    if (potential_x >= 20) {
      return;
    }
  } else {
  }

  if (y_pos <= y_min && (map_->y == 0.0F)) {
    y_pos_min = 0;
    if (potential_y >= 20) {
      return;
    }
  } else if (y_pos >= y_max && (map_->y == static_cast<float>(this->map_->y_max_ - this->frame_config_.rows))) {
    y_pos_max = frame_config_.rows - 1;
    if (potential_y >= 20) {
      return;
    }
  } else {
  }

  switch (direction) {
    case map::RIGHT :
      if (x_pos >= x_pos_max) {
        movement.MoveSmooth(1, 0, true);
      } else {
        movement.MoveSmooth(1, 0);
      }

      animation.Play_animation(8);
      break;
    case map::LEFT:
      if (x_pos <= x_pos_min) {
        movement.MoveSmooth(-1, 0, true);
      } else {
        movement.MoveSmooth(-1, 0);
      }

      animation.Play_animation(4);
      break;
    case map::DOWN:
      if (y_pos >= y_pos_max) {
        movement.MoveSmooth(0, 1, true);
      } else {
        movement.MoveSmooth(0, 1);
      }

      animation.Play_animation(0);
      break;
    case map::UP:
      if (y_pos <= y_pos_min) {
        movement.MoveSmooth(0, -1, true);
      } else {
        movement.MoveSmooth(0, -1);
      }

      animation.Play_animation(12);
      break;
  }
}

void CharacterPlayer::Render() {
  // Render player
  const auto TILE_DIM = static_cast<float>(this->frame_config_.tile_dimentions);
  const float PLAYER_X = static_cast<float>(this->frame_config_.offset_x) + (movement.x * TILE_DIM);
  const float PLAYER_Y = static_cast<float>(this->frame_config_.offset_y) + (movement.y * TILE_DIM);
  const float OFFSET_Y = 20.0F;

  //printf("rx:%f, ry:%f\n", rx, ry);
  //pTexture->Render(animation.current_frame, &pRenderer, rx, ry);

  Texture::Render(
      *sdl::renderer,
      texture,
      animation.current_frame,
      static_cast<int>(PLAYER_X),
      static_cast<int>(PLAYER_Y - OFFSET_Y)
  );

  // Player box
  if (player::draw_bounds) {
    auto player_box = SDL_Rect{
        x_pos_min * frame_config_.tile_dimentions,
        y_pos_min * frame_config_.tile_dimentions,
        ((x_pos_max - x_pos_min) * frame_config_.tile_dimentions) + frame_config_.tile_dimentions,
        ((y_pos_max - y_pos_min) * frame_config_.tile_dimentions) + frame_config_.tile_dimentions
    };
    auto player_character_box = SDL_Rect{
        static_cast<int>(PLAYER_X),
        static_cast<int>(PLAYER_Y),
        frame_config_.tile_dimentions,
        frame_config_.tile_dimentions,
    };
    auto map_box = SDL_Rect{
        frame_config_.offset_x,
        frame_config_.offset_y,
        frame_config_.width,
        frame_config_.height
    };

    SDL_SetRenderDrawColor(sdl::renderer, 0xFF, 0x00, 0x00, 0xFF);

    SDL_RenderDrawRect(sdl::renderer, &player_box);
    SDL_RenderDrawRect(sdl::renderer, &player_character_box);
    SDL_RenderDrawRect(sdl::renderer, &map_box);
  }
}
