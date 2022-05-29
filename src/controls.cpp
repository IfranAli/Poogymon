#include "controls.h"
#include "UI/menu.h"
#include "input/editor.h"
#include "input/player.h"
#include "input/text_select.h"

namespace input
{
	void init_input_maps()
	{
		menu::init_input_maps();
      editor::Init();
		player::init();
        text_select::init();

		puts("Map initialization complete.");
	}

} /* input */
