#include "editor.h"
#include "../Map.h"
#include "../InputContext.h"
#include "dbgmenu.h"
#include "../Renderer.h"
#include "../UI/menu.h"

namespace editor {

  namespace generate {
    void MapNamesToFunc(::input::input_fn exec_func) {
      auto filenames = map::active_map->GetMapConnections();

      const auto MAX_CONNECTIONS = static_cast<size_t>(5);
      input::input_fn fn[MAX_CONNECTIONS];

      for (auto i = static_cast<size_t>(0); i < MAX_CONNECTIONS; i++) {
        fn[i] = exec_func;
      }

      menu::AddMenu(std::move(filenames), +fn);
    }
  }

  input::InputHandler editor;

  int selected_tile = 0;
  bool show_tileset = false;

  void GetTile() {
    auto x = input::mouse_x;
    auto y = input::mouse_y;
    auto result = 0;

    if (editor::show_tileset && (x < config::SCREEN_OFFSET_X)) {
      auto &tile_set = map::active_map->active->map_tileset;
      result = tile_set.GetTile(x, y);
    } else {
      result = map::active_map->GetTileFromMouse(x, y);
    }
    std::printf("got tile %d\n", result);

    if (result >= 0) {
      selected_tile = result;
    }
  }

  void PlaceTile() {
    map::active_map->SetTile(::input::mouse_x, ::input::mouse_y, selected_tile);
  }

  void DrawTileset() {
    if (editor::show_tileset) {
      auto &texture = map::active_map->active->map_tileset.texture;
      // TODO render guard.
      SDL_Rect clip;
      clip.x = 0;
      clip.y = 0;
      clip.w = texture.mWidth;
      clip.h = config::SCREEN_HEIGHT;

      Texture::Render(
          texture,
          *sdl::renderer,
          0,
          0,
          &clip);

    }
  }

  void OnEnable() {
    editor::show_tileset = true;
    if (editor::show_tileset) {
      auto texture_width = map::active_map->active->map_tileset.texture.mWidth;

// TODO: Maybe look into passing dimentions like this.
//    SDL_Rect dimentions = {
//        .x = texture_width + ::config::SCREEN_WIDTH,
//        .y = ::config::SCREEN_HEIGHT,
//        .w = texture_width
//    };

      ::config::RecalculateWindowVariables(texture_width + ::config::SCREEN_WIDTH,
                                           ::config::SCREEN_HEIGHT,
                                           texture_width);
    }
  }

  void OnDisable() {
    if (editor::show_tileset) {
      editor::show_tileset = false;
      ::config::RecalculateWindowVariables();
    }
  }

  void MapSaveDo() {
    map::active_map->SaveMap(menu::lastSelectedIndex());
  }

  void MapLoadDo() {
    map::active_map->LoadMap(menu::lastSelectedIndex());
  }

  void MapPrintDo() {
    map::active_map->PrintMap(menu::lastSelectedIndex());
  }

  void MapPrint() {
    generate::MapNamesToFunc(MapPrintDo);
  }

  void MapLoad() {
    generate::MapNamesToFunc(MapLoadDo);
  }

  void MapSave() {
    generate::MapNamesToFunc(MapSaveDo);
  }

  void CloseMenu() {
    input::SetPrevHandler();
    menu::menu_close();
  }

  void OpenMenu() {
    input::input_fn fn[]{
        MapPrint,
        MapLoad,
        MapSave,
    };

    menu::AddMenu(
        {"Print", "Load", "Save"}, fn
    );
  }

  void BtnUp() {
    if (::map::active_map->CanMoveUp()) {
      ::map::MoveSmooth(0, -1, true);
    }
  }

  void BtnDown() {
    if (::map::active_map->CanMoveDown()) {
      ::map::MoveSmooth(0, 1, true);
    }
  }

  void BtnLeft() {
    if (::map::active_map->CanMoveLeft()) {
      ::map::MoveSmooth(-1, 0, true);
    }
  }

  void BtnRight() {
    if (::map::active_map->CanMoveRight()) {
      ::map::MoveSmooth(1, 0, true);
    }
  }

  void Init() {
    editor.fn[::input::ON_ENABLE] = OnEnable;
    editor.fn[::input::ON_DISABLE] = OnDisable;
    editor.fn[::input::UP] = BtnUp;
    editor.fn[::input::DOWN] = BtnDown;
    editor.fn[::input::LEFT] = BtnLeft;
    editor.fn[::input::RIGHT] = BtnRight;
    editor.fn[::input::START] = debug::open_menu;
//  editor.[::input::n[SELECT]     = editor::CloseMenu;
    editor.fn[::input::L] = []() { editor::selected_tile--; };
    editor.fn[::input::R] = []() { editor::selected_tile++; };
    editor.fn[::input::MOUSE_R] = editor::GetTile;
    editor.fn[::input::MOUSE_L] = editor::PlaceTile;

    constexpr uint32_t EDITOR_MASK = input::gen_mask(
        {
            input::KEY::ON_ENABLE,
            input::KEY::ON_DISABLE,
            input::UP,
            input::DOWN,
            input::LEFT,
            input::RIGHT,
            input::START,
            input::L,
            input::R,
            input::MOUSE_R,
            input::MOUSE_L}
    );

    editor.mask = EDITOR_MASK;
  }

} /* editor */
