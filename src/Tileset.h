#pragma once

#include "Texture.h"

namespace Texture {
	struct tileset_info final {
		int tile_per_row;
		int tile_width;
		int tile_height;
	};

	/* utility class combines tileset_info and texture */
	struct Tileset final
	{
		public:
			Tileset();
			~Tileset();

			Tileset (const Tileset& t) noexcept;
			Tileset(Tileset&&) noexcept ;
			Tileset& operator= (Tileset&&) noexcept ;
            int GetTile(int x = 0, int y = 0) const;

			Texture texture;
			tileset_info info{};
	};


	bool LoadFromFile(SDL_Renderer& r, Texture& t, tileset_info& i, const char* path, int w, int h);
	bool LoadFromFile(SDL_Renderer& r, Tileset& t, const char* path, int w, int h);

	void Render(SDL_Renderer& r, const Texture& texture, const tileset_info& info, int i, int x, int y);
	void Render(SDL_Renderer& r, const Tileset& tileset, int tile, int x, int y);

	void getTile(const Texture& texture, const tileset_info& info, int i, SDL_Rect& clip);

  tileset_info getTilesetInfo(const Texture& texture, int w, int h);

} /* texture */
