#pragma once

#include <cstdint>
#include <initializer_list>

namespace input
{
	using input_fn = void (*)();

	enum KEY : unsigned char 
	{
		ON_ENABLE,
		ON_DISABLE,
		A,
		B,
		L,
		R,
		START,
		SELECT,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		ARROW_UP,
		ARROW_DOWN,
		ARROW_LEFT,
		ARROW_RIGHT,
		MOUSE_L, 
		MOUSE_R,

		A_RELEASE,
		B_RELEASE,
		UP_RELEASE,
		DOWN_RELEASE,
		LEFT_RELEASE,
		RIGHT_RELEASE,
		L_RELEASE,
		R_RELEASE,
		START_RELEASE,
		SELECT_RELEASE,
		NONE
	};

	constexpr unsigned int MAX_KEYS =  KEY::NONE;

	struct InputHandler
	{
		input_fn fn[MAX_KEYS];
		uint32_t mask;
	};

	constexpr uint32_t gen_mask (std::initializer_list<KEY> keys)
	{
		uint32_t s {0};
		for ( auto& k : keys ) s |= (1 << k);
		return s;
	}

} /* input */
