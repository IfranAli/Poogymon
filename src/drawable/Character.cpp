#include <iostream>

#include "Character.h"

Character::Character() {
  /* Player speed_ and animation speed_ */
  movement_.speed_ = default_movement_speed_;
  animation_.Set_animation_speed(default_animation_speed_);
}

Character::~Character() = default;

void Character::Tick() {
  if (animation_.currently_playing) {
    animation_.Tick();
  }

  if (movement_.moving_) {
    movement_.Step();

    // Has movement_ ceased
    if (!movement_.moving_) {

      // Stop all animations
      if (animation_.currently_playing) {
        animation_.Stop_animation();
      }

      printf("Player X: %f, Y: %f\n", movement_.x_, movement_.y_);
    }

  } else if (velocity_x_ != 0 || velocity_y_ != 0) {

    movement_.MoveSmooth(velocity_x_, velocity_y_);

    if (movement_.direction_ == 1) {
      if (movement_.pole_ > 0) {
        puts("Moving RIGHT\n");
        animation_.Play_animation(8);
      } else {
        puts("Moving DOWN\n");
        animation_.Play_animation(0);
      }
    } else {
      if (movement_.pole_ > 0) {
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
  movement_.speed_ = default_movement_speed_;
}

