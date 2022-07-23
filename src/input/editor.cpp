#include "editor.h"
#include "../InputManager.h"
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
  namespace {
    int selected_tile = 0;
    bool show_tileset = false;
    RenderManager *p_render_manager = nullptr;
    CharacterPlayer *p_character_player = nullptr;
    map::Map *p_map = nullptr;
  }

  void GetTile() {
    auto x = input::mouse_x;
    auto y = input::mouse_y;
    auto result = 0;

    if (editor::show_tileset && (x < sdl::g_frame_config->GetOffsetX())) {
      auto &tile_set = map::active_map->GetActiveMapData().map_tileset_;
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
    // todo: implement new method to draw maps if they have pending changes.
    map::active_map->GetActiveMapData().DrawMap();
  }

  void DrawTileset() {
    if (editor::show_tileset) {
      FrameConfig frame_config;
      auto &texture = map::active_map->GetActiveMapData().map_tileset_.texture;

      SDL_Rect src;
      src.x = src.y = 0;
      src.w = texture.mWidth;
      src.h = frame_config.GetHeight();

      SDL_Rect dest = src;

      SDL_RenderCopy(sdl::renderer, texture.mTexture, &src, &dest);
    }
  }

  void OnEnable() {
    editor::show_tileset = true;
    if (editor::show_tileset) {
      auto tileset_width = map::active_map->GetActiveMapData().map_tileset_.texture.mWidth;

      sdl::g_frame_config->RecalculateWindowVariables(DimensionType{
          .width = sdl::g_frame_config->GetWidth() + tileset_width,
          .height = sdl::g_frame_config->GetHeight(),
      });
    }

    p_character_player->GetMovementObject().speed = 16;
    p_character_player->SetMoveBoundary(0, 0);
  }

  void OnDisable() {
    if (editor::show_tileset) {
      editor::show_tileset = false;
      sdl::g_frame_config->RecalculateWindowVariables();

      p_character_player->ResetSpeed();
      p_character_player->ResetBoundingBox();
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
    p_character_player->MovePlayer(map::UP);
  }

  void BtnDown() {
    p_character_player->MovePlayer(map::DOWN);
  }

  void BtnLeft() {
    p_character_player->MovePlayer(map::LEFT);
  }

  void BtnRight() {
    p_character_player->MovePlayer(map::RIGHT);
  }

  void Init(RenderManager *render_manager, CharacterPlayer *character_player, map::Map *map) {
    p_render_manager = render_manager;
    p_character_player = character_player;
    p_map = map;

    editor::editor.fn[::input::ON_ENABLE] = OnEnable;
    editor::editor.fn[::input::ON_DISABLE] = OnDisable;
    editor::editor.fn[::input::UP] = BtnUp;
    editor::editor.fn[::input::DOWN] = BtnDown;
    editor::editor.fn[::input::LEFT] = BtnLeft;
    editor::editor.fn[::input::RIGHT] = BtnRight;
    editor::editor.fn[::input::START] = debug::open_menu;
//  editor::editor.[::input::n[SELECT]     = editor::CloseMenu;
    editor::editor.fn[::input::L] = []() { editor::selected_tile--; };
    editor::editor.fn[::input::R] = []() { editor::selected_tile++; };
    editor::editor.fn[::input::MOUSE_R] = editor::GetTile;
    editor::editor.fn[::input::MOUSE_L] = editor::PlaceTile;

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

    editor::editor.mask = EDITOR_MASK;
  }

} /* editor */
