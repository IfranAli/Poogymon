#include "CharacterPlayer.h"
#include "../Renderer.h"
#include "../Map.h"

#include <iostream>

namespace player {
  CharacterPlayer *player{nullptr};
  bool is_visible{false};
  bool draw_bounds{false};
}

void CharacterPlayer::ResetBoundingBox() {
  /* Do not want neg values */
  if (mov_boundary_x > ::config::TILE_PER_COL) { mov_boundary_x = ::config::TILE_PER_COL; }
  if (mov_boundary_y > ::config::TILE_PER_ROW) { mov_boundary_y = ::config::TILE_PER_ROW; }

  /* Calc player limits */
  x_max = ((::config::TILE_PER_COL + mov_boundary_x) / 2);
  y_max = ((::config::TILE_PER_ROW + mov_boundary_y) / 2);
  x_min = x_max - mov_boundary_x;
  y_min = y_max - mov_boundary_y;

  /* Calc rect for drawing bounding box */
  player_min_x = ::config::SCREEN_OFFSET_X + (x_min * ::config::TILE_DIM);
  player_min_y = ::config::SCREEN_OFFSET_Y + (y_min * ::config::TILE_DIM);
  player_max_x = (mov_boundary_x * ::config::TILE_DIM);
  player_max_y = (mov_boundary_y * ::config::TILE_DIM);

  /* Place character inside bounds */
  Character::movement.x = static_cast<float>(x_min);
  Character::movement.y = static_cast<float>(y_min);

  printf("Player - bounding box update: W:%lu, H:%lu\n", mov_boundary_x, mov_boundary_y);
}

CharacterPlayer::~CharacterPlayer() = default;

CharacterPlayer::CharacterPlayer() {
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

  switch (direction) {
    case map::RIGHT:
//      if (movement.x == x_max && map::active_map->CanMoveRight()) {
        movement.MoveSmooth(1, 0, true);
//      } else {
//        movement.MoveSmooth(1, 0);
//      }
      animation.Play_animation(8);
      break;

    case map::LEFT:
//      if (movement.x == x_min && map::active_map->CanMoveLeft()) {
        movement.MoveSmooth(-1, 0, true);
//      } else {
//        movement.MoveSmooth(-1, 0);
//      }
      animation.Play_animation(4);
      break;

    case map::DOWN:
      // Can move down?
//      if (movement.y == y_max && map::active_map->CanMoveDown()) {
        movement.MoveSmooth(0, 1, true);
//      } else {
//        movement.MoveSmooth(0, 1);
//      }
      animation.Play_animation(0);

      break;

    case map::UP:
      // Can move up?
//      if (movement.y == y_min && map::active_map->GetY() != 0) {
        //map::active_map->MoveSmooth(0, -1);
        movement.MoveSmooth(0, -1, true);
//      } else {
//        movement.MoveSmooth(0, -1);
//      }
      animation.Play_animation(12);
      break;
  }
}

void CharacterPlayer::Render() {
  // Render player
  float rx = config::SCREEN_OFFSET_X + (movement.x * config::TILE_DIM);
  float ry = config::SCREEN_OFFSET_Y + (movement.y * config::TILE_DIM) - 16;

  //printf("rx:%f, ry:%f\n", rx, ry);
  //pTexture->Render(animation.current_frame, &pRenderer, rx, ry);

  Texture::Render(*sdl::renderer, texture, animation.current_frame, rx, ry);

  // Player box
  if (player::draw_bounds) {
    auto player_box = SDL_Rect{player_min_x, player_min_y, player_max_x + 32, player_max_y + 32};
    auto player_texture_box = SDL_Rect{(int) rx, (int) ry + 16, 32, 32};
    auto map_box = SDL_Rect{0, 0, config::SCREEN_WIDTH, config::SCREEN_HEIGHT};

    SDL_SetRenderDrawColor(sdl::renderer, 0xFF, 0x00, 0x00, 0xFF);
    render::DrawRectangle(player_box);
    render::DrawRectangle(player_texture_box, false);
    render::DrawRectangle(map_box);
  }
}
