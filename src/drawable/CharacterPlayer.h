#ifndef CHARACTER_PLAYER_H 
#define CHARACTER_PLAYER_H 

#include "Character.h"
#include "../Tileset.h"

class CharacterPlayer final: public Character
{
	public:
		CharacterPlayer();
		~CharacterPlayer();

		void Tick() override;
		void Render() override;

		void ResetBoundingBox();

		int mov_boundary_x = 4;
		int mov_boundary_y = 2;

	private:
		const char* texture_path = "Resource/red.png";
		Texture::Tileset texture;

		int x_max {};
		int y_max {};
		int x_min {};
		int y_min {};

		int player_min_x = {};
		int player_min_y = {};
		int player_max_x = {};
		int player_max_y = {};
};

namespace player
{
	extern CharacterPlayer* player;
	extern bool is_visible;
	extern bool draw_bounds;
}

#endif // CHARACTER_PLAYER_H
