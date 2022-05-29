#include "Animation.h"
#include "../Renderer.h"

bool Animation::Play_animation(int anim_group) {

	/* Return if re-applying animation */
	if (currently_playing && animation_group == anim_group) {
		return false;
	}

	animation_group = anim_group;
	current_frame = animation_group + 1;
	animation_group_max = anim_group + 4; // Actual + 1

	current_frame_delta = animation_group;
	currently_playing = true;

	return true;
}

void Animation::Stop_animation() {
	current_frame = animation_group;
	currently_playing = false;
}

void Animation::Set_animation_speed(float new_speed) {
	speed = new_speed;
}

void Animation::Tick() {
	current_frame_delta += speed * sdl::deltatime;

	// Loop animation 
	if (current_frame_delta >= animation_group_max) current_frame_delta = animation_group;

	current_frame = current_frame_delta; // Integer cast
	//printf("D: %f, CURRENT ANIM GROUP: %d, CURRENT FRAME: %d, MAX FRAME: %d\n", current_frame_delta, animation_group, current_frame, animation_group_max);
}
