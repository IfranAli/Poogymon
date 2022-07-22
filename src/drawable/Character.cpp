#include <iostream>

#include "Character.h"

Character::Character() {
  /* Player speed and animation speed */
  movement_.speed = default_movement_speed_;
  animation_.Set_animation_speed(default_animation_speed_);
}

Character::~Character() = default;

void Character::Tick() {
  if (animation_.currently_playing) {
    animation_.Tick();
  }

  if (movement_.moving) {
    movement_.Step();

    // Has movement_ ceased
    if (!movement_.moving) {

      // Stop all animations
      if (animation_.currently_playing) {
        animation_.Stop_animation();
      }

      printf("Player X: %f, Y: %f\n", movement_.x, movement_.y);
    }

  } else if (velocity_x_ != 0 || velocity_y_ != 0) {

    movement_.MoveSmooth(velocity_x_, velocity_y_);

    if (movement_.direction == 1) {
      if (movement_.pole > 0) {
        puts("Moving RIGHT\n");
        animation_.Play_animation(8);
      } else {
        puts("Moving DOWN\n");
        animation_.Play_animation(0);
      }
    } else {
      if (movement_.pole > 0) {
        puts("Moving LEFT\n");
        animation_.Play_animation(4);
      } else {
        puts("Moving UP\n");
        animation_.Play_animation(12);
      }
    }
  }
}
void Character::ResetSpeed() {
  movement_.speed = default_movement_speed_;
}

