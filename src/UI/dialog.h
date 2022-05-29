#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "../Texture.h"

// TODO: Find a way to not have std includes in header
#include <vector>
#include <string>

namespace dialog
{
	void close();
	bool init();

	struct menu_config
	{
		SDL_Rect dim;
		SDL_Rect dim_selected;
		Texture::Texture texture;
	};

	bool draw (menu_config& c);
	void set_selectedIndex (menu_config& c, size_t i);
	void render_menu (menu_config& c, const std::vector<std::string>& entries);
	void place_menu_relative(menu_config& m1, menu_config& m2);


	void add_item(const float& rv, size_t x, size_t y);
	void update_labels(float dt);
}

