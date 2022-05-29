#include "dialog.h"

#include "../Renderer.h"
#include <iostream>
#include <vector>

namespace dialog
{
	TTF_Font* pFont {nullptr};

	struct label {
		std::string text;
		SDL_Texture* texture {NULL};
		SDL_Rect render_quad;
		const float& val;
		label(const float&v): val(v) {};
		~label() { SDL_DestroyTexture(texture); }
	};
	std::vector<label> labels;	

	void close()
	{
		labels.clear();
		TTF_CloseFont(pFont);
		TTF_Quit();
	}

	bool init()
	{
		constexpr char font_file_name[] = "lib.ttf";
		constexpr size_t sz = 16;

		return (!TTF_Init()) && ((pFont = TTF_OpenFont(font_file_name, sz)) != NULL);
	}

	SDL_Texture* text_to_texture(
			SDL_Renderer* r,
			const char* tx, 
			SDL_Rect& quad, 
			SDL_Color colour = SDL_Color { 0, 0, 0, 255 })
	{
		SDL_Surface* surface = TTF_RenderText_Solid(pFont, tx, colour);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface); 
		//quad.x = 0;
		//quad.y = 0;
		quad.w = surface->w;
		quad.h = surface->h;
		SDL_FreeSurface(surface);
		return texture;
	}


	constexpr size_t row_item_height = 19;

	void render_menu(menu_config& c, const std::vector<std::string>& names)
	{
		size_t width = 150;
		size_t height = row_item_height * names.size();

		Texture::MakeEmpty(c.texture, *sdl::renderer, width, height);
		Texture::SetAsRenderTarget(c.texture, *sdl::renderer);

		SDL_SetRenderDrawColor(sdl::renderer, 0xD3, 0xD3, 0xD3, 0xFF );
		SDL_RenderClear(sdl::renderer);

		SDL_Texture* t;
		int y = 0;
		for (auto& n : names) {
			t = text_to_texture(sdl::renderer, n.c_str(), c.dim);

			c.dim.y = y;
			y += c.dim.h;

			SDL_RenderCopy(sdl::renderer, t, NULL, &c.dim);
			SDL_DestroyTexture(t);
		}

		SDL_SetRenderTarget(sdl::renderer, NULL );
		
		c.dim.x = 0;
		c.dim.y = 0;

		c.dim.w = c.texture.mWidth;
		c.dim.h = c.texture.mHeight;

		c.dim_selected.x = 0;
		c.dim_selected.w = c.dim.w;
		c.dim_selected.h = row_item_height; 
	}

	void place_menu_relative(menu_config& m1, menu_config& m2) {
		m1.dim.x = (m2.dim.x + m2.dim.w);
		m1.dim.y = (m2.dim_selected.y);
		m1.dim_selected.x = m1.dim.x;
		set_selectedIndex(m1, 0);
	}


	void set_selectedIndex(menu_config& c, size_t i) {
		c.dim_selected.y = c.dim.y + (i * row_item_height);
	}

	bool draw(menu_config& c) {
      Texture::Render(c.texture, *sdl::renderer, &c.dim, NULL);
      SDL_SetRenderDrawColor(sdl::renderer, 0xFF, 0x00, 0x00, 0xFF);
      SDL_RenderDrawRect(sdl::renderer, &c.dim_selected);
      return true;
	}

	// Quick labels
	//
		
	void set_label_xy (label& l, size_t x, size_t y) {
		l.render_quad.x = x;
		l.render_quad.y = y;
	}

	void add_item(const float& rv, size_t x, size_t y) {
		labels.emplace_back(rv);
		label& l = labels.back();
		l.render_quad.x = x;
		l.render_quad.y = y;

		l.text = std::to_string(l.val);
		l.texture = text_to_texture(sdl::renderer, l.text.c_str(), l.render_quad);
	}

	float elapsed = 0;
	void update_labels(float dt) {
		elapsed += dt;
		for (auto& l : labels)
		{
			if (elapsed > 1)
			{
				l.text = std::to_string(l.val);
				if (l.texture != NULL) SDL_DestroyTexture(l.texture);
				l.texture = text_to_texture(sdl::renderer, l.text.c_str(), l.render_quad);
				elapsed = 0;
			}
			SDL_RenderCopy(sdl::renderer, l.texture, NULL, &l.render_quad);
		}
	}



} /* dialog */
