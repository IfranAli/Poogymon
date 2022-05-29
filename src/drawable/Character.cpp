#include <iostream>

#include "Character.h"


Character::Character() {
    mVelocityX = 0;
    mVelocityY = 0;

	/* Player speed and animation speed */
	movement.speed = 0.05f;
	animation.Set_animation_speed(0.20);
}

Character::~Character() = default;


void Character::Tick()
{
	if (animation.currently_playing) animation.Tick();

	if (movement.moving) {
		movement.Step();

		// Has movement ceased 
		if (!movement.moving) {
			// Stop all animations
			if (animation.currently_playing) animation.Stop_animation();
			printf("Player X: %f, Y: %f\n", movement.x, movement.y);
		}

	}
	else if (mVelocityX != 0 || mVelocityY != 0) {

		movement.MoveSmooth(mVelocityX, mVelocityY);

		//printf("POLE: %d, Direction: %d\n", movement.pole, movement.direction);
		if (movement.direction == 1) {
			if (movement.pole > 0) {
				printf("Moving RIGHT\n");
				animation.Play_animation(8);
			} else {
				printf("Moving DOWN\n");
				animation.Play_animation(0);
			}
		} else {
			if (movement.pole > 0) {
				printf("Moving LEFT\n");
				animation.Play_animation(4);
			} else {
				printf("Moving UP\n");
				animation.Play_animation(12);
			}
		}
	}
}

