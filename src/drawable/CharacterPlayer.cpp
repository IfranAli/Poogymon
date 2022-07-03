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

  /* Do not want neg values */
  if (mov_boundary_x > config.cols) { mov_boundary_x = config.cols; }
  if (mov_boundary_y > config.rows) { mov_boundary_y = config.rows; }

  /* Calc player limits */
  x_max = ((config.cols + mov_boundary_x) / 2);
  y_max = ((config.rows + mov_boundary_y) / 2);
  x_min = x_max - mov_boundary_x;
  y_min = y_max - mov_boundary_y;

  /* Calc rect for drawing bounding box */
  // TODO:  screen offsets
  player_min_x = config.offset_x + (x_min * config.tile_dimentions);
  player_min_y = config.offset_y + (y_min * config.tile_dimentions);
  player_max_x = (mov_boundary_x * config.tile_dimentions);
  player_max_y = (mov_boundary_y * config.tile_dimentions);

  /* Place character inside bounds */
  Character::movement.x = static_cast<float>(x_min);
  Character::movement.y = static_cast<float>(y_min);

  printf("Player - bounding box update: W:%lu, H:%lu\n", mov_boundary_x, mov_boundary_y);
}

CharacterPlayer::~CharacterPlayer() = default;

CharacterPlayer::CharacterPlayer(FrameConfig frame_config) : frame_config_(frame_config) {
  Texture::LoadFromFile(*sdl::renderer, texture, texture_path, 32, 48);
  ResetBoundingBox();
}

constexpr float base_animation_speed = 11;
constexpr float base_movement_speed = 3;
constexpr float running_speed = 1.8;
constexpr float run_animation_speed = base_animation_speed * running_speed;
constexpr float run_movement_speed = base_movement_speed * running_speed;

void CharacterPlayer::Tick() {
  //printf("Map X: %direction, Y: %direction\n", map.GetX(), map.GetY());

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

  switch (direction) {
    //TODO: These variables arenot good.
    case map::RIGHT:

      if (movement.x == x_max) {
        movement.MoveSmooth(1, 0);
        animation.Play_animation(8);
      }

      if (potential_x > x_max) {
        movement.MoveSmooth(1, 0);
        animation.Play_animation(8);
        return;
      }

      if (potential_x < x_max) {
        movement.MoveSmooth(1, 0);
        animation.Play_animation(8);
      } else {
        movement.MoveSmooth(1, 0, true);
        animation.Play_animation(8);
      }

      break;

    case map::LEFT:
      if (potential_x >= 0 && potential_x <= 20) {
        if (potential_x == x_min) {
          movement.MoveSmooth(-1, 0, true);
        } else {
          movement.MoveSmooth(-1, 0);
        }
      }

      animation.Play_animation(4);
      break;

    case map::DOWN:
//       Can move down?
      if (movement.y == y_max && canMoveDown) {
        movement.MoveSmooth(0, 1, true);
      } else {
        movement.MoveSmooth(0, 1);
      }
      animation.Play_animation(0);

      break;

    case map::UP:
      // Can move up?
      if (movement.y == y_min && canMoveUp) {
        movement.MoveSmooth(0, -1, true);
      } else {
        movement.MoveSmooth(0, -1);
      }
      animation.Play_animation(12);
      break;

//      map::active_map->x = movement.x;
//      map::active_map->y = movement.x;
  }
}

void CharacterPlayer::Render() {
  // Render player
  float rx = frame_config_.offset_x + (movement.x * frame_config_.tile_dimentions);
  float ry = frame_config_.offset_y + (movement.y * frame_config_.tile_dimentions) + 16;

  //printf("rx:%f, ry:%f\n", rx, ry);
  //pTexture->Render(animation.current_frame, &pRenderer, rx, ry);

  Texture::Render(*sdl::renderer, texture, animation.current_frame, rx, ry);

  // Player box
  if (player::draw_bounds) {
    auto player_box = SDL_Rect{player_min_x, player_min_y, player_max_x + 32, player_max_y + 32};
    auto player_texture_box = SDL_Rect{(int) rx, (int) ry, 32, 32};
    auto map_box = SDL_Rect{
      frame_config_.offset_x,
      frame_config_.offset_y,
      frame_config_.width,
      frame_config_.height
    };

    SDL_SetRenderDrawColor(sdl::renderer, 0xFF, 0x00, 0x00, 0xFF);

    SDL_RenderDrawRect(sdl::renderer, &player_box);
    SDL_RenderDrawRect(sdl::renderer, &player_texture_box);
    SDL_RenderDrawRect(sdl::renderer, &map_box);
  }
}
