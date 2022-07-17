#include "Map.h"
#include "Renderer.h"
#include "utility/Utility.h"

#include <SDL.h>
#include <iostream>
#include <sstream>
#include <utility>

namespace map {
  int pole{0}; // -1 = UP, LEFT
  int direction{0};
  bool moving{false};
  float target{0};
  float *axis{nullptr};
  float x;
  float y;
  float speed{transition_speed_default};
  Map *active_map;

  auto index_to_enum = [](int index = 0) -> MapIndex {
    switch (index) {
      case 0:return MapIndex::ACTIVE;
      case 1:return MapIndex::LEFT_CONNECTION;
      case 2:return MapIndex::RIGHT_CONNECTION;
      case 3:return MapIndex::TOP_CONNECTION;
      case 4:return MapIndex::BOTTOM_CONNECTION;
      default:return MapIndex::ACTIVE;
    }
  };

  std::vector<MapMetaData> ParseMapFile(const std::string &filename, size_t sz = 0) {
    std::ifstream ifs(filename);
    std::vector<MapMetaData> list{};
    list.reserve(sz);
    std::string buffer{};

    auto index = 0;
    while (std::getline(ifs, buffer)) {
      if (buffer.empty()) {
        index++;
        continue;
      }
      std::stringstream ss{buffer};
      MapMetaData map_meta_data{};
      map_meta_data.connection_type = index_to_enum(index++);
      ss >> map_meta_data.filename;

      while (ss >> buffer) {
        std::string texture_file{buffer};
        map_meta_data.texture_filenames.emplace_back(texture_file);
      }

      list.emplace_back(map_meta_data);
    }

    return list;
  }

  map_data::MapData &Map::GetConnection(MapIndex type) {
    switch (type) {
      case ACTIVE:return map_data_list[ACTIVE];
        break;
      case LEFT_CONNECTION:return *map_data_list[MapIndex::ACTIVE].connections_.Left;
        break;
      case RIGHT_CONNECTION:return *map_data_list[MapIndex::ACTIVE].connections_.Right;
        break;
      case TOP_CONNECTION:return *map_data_list[MapIndex::ACTIVE].connections_.Up;
        break;
      case BOTTOM_CONNECTION:return *map_data_list[MapIndex::ACTIVE].connections_.Down;
        break;
    }
  }

  bool Map::CanMoveUp() const {
    return ((y - 1) >= y_min_);
  }

  bool Map::CanMoveDown() const {
    auto result = ((y + frame_config_.GetRows() + 1) <= y_max_);
    return result;
  }

  bool Map::CanMoveLeft() const {
    auto result = (x - 1) > x_min_;
    return result;
  }

  bool Map::CanMoveRight() const {
    return ((x + frame_config_.GetCols() + 1) <= x_max_);
  }

  bool CanMove(Direction move_direction) {
    switch (move_direction) {
      case UP: return active_map->CanMoveUp();
      case DOWN:return active_map->CanMoveDown();
      case LEFT:return active_map->CanMoveLeft();
      case RIGHT:return active_map->CanMoveRight();
    }
  }

  void MoveSmooth(int vx, int vy, bool move_camera) {
    if (moving)
      return;

    direction = (vx ? vx : vy);
    pole = (vx ? 1 : -1);

    if (move_camera) {
      axis = (vx ? &map::active_map->x : &map::active_map->y);
    } else {
      axis = (vx ? &x : &y);
    }

    target = *axis + (direction);
    *axis += (direction * speed) * sdl::deltatime;
    moving = true;
  }

  void MoveSmoothStep() {
    *axis += (direction * speed) * sdl::deltatime;
    if ((direction > 0 && *axis > target) || (direction < 0 && *axis < target)) {
      *axis = target;
      moving = false;

//      active_map->active->DrawMap(x, y);
    }
  }

  Map::Map(FrameConfig &frame_config) : frame_config_(frame_config) {
    map_data::TilePattern tile_pattern_1(0);
    map_data::TilePattern tile_pattern_2(11, 0, 0, 11);
    map_data::TilePattern tile_pattern_3(2);
    map_data::TilePattern tile_pattern_4(8);
    map_data::TilePattern tile_pattern_5(25);

    map_data_list[MapIndex::ACTIVE] = map_data::MapData("tile.png", tile_pattern_1);
    map_data_list[MapIndex::RIGHT_CONNECTION] = map_data::MapData("tile.png", tile_pattern_2);
    map_data_list[MapIndex::LEFT_CONNECTION] = map_data::MapData("tile.png", tile_pattern_3);
    map_data_list[MapIndex::BOTTOM_CONNECTION] = map_data::MapData("tile.png", tile_pattern_4);
    map_data_list[MapIndex::TOP_CONNECTION] = map_data::MapData("tile.png", tile_pattern_5);

    map_data_list[ACTIVE].connections_.Right = &map_data_list[RIGHT_CONNECTION];
    map_data_list[ACTIVE].connections_.Left = &map_data_list[LEFT_CONNECTION];
    map_data_list[ACTIVE].connections_.Down = &map_data_list[BOTTOM_CONNECTION];
    map_data_list[ACTIVE].connections_.Up = &map_data_list[TOP_CONNECTION];

    AddMapData(map_data_list[ACTIVE]);
    moving = false;
  }

  Map::~Map() {
    printf("MAP -dtor\n");
  }

  Map &Map::operator=(Map &&t) noexcept {
    return *this;
  }

  bool Map::HasConnection(MapIndex type) {
    return map_data_list[type].IsValidMap();
  }

  void Map::Tick() const {
    if (show_tile_set_) {
      // TODO: Implement Function..
    }

    if (moving) {
      ::map::MoveSmoothStep();
    }
  }

  void Map::AddMapData(map_data::MapData &mapdata) {
    active = &mapdata;
    this->total_width_ += mapdata.GetMapWidth();
    this->total_height_ += mapdata.GetMapHeight();
    this->x_max_ += total_width_;
    this->y_max_ += total_height_;

    if (active->connections_.Right != nullptr) {
      auto width = active->connections_.Right->GetMapWidth();
      this->total_width_ += width;
      this->x_max_ += width;
    }
    if (active->connections_.Left != nullptr) {
      auto width = active->connections_.Left->GetMapWidth();
      this->total_width_ += width;
      this->x_min_ -= width;
    }
    if (active->connections_.Up != nullptr) {
      auto width = active->connections_.Up->GetMapWidth();
      this->total_height_ += width;
      this->y_min_ -= width;
    }
    if (active->connections_.Down != nullptr) {
      auto height = active->connections_.Down->GetMapHeight();
      this->total_height_ += height;
      this->y_max_ += height;
    }

    printf("Total Width: %d\nTotal Height: %d\n", this->total_width_, this->total_height_);
  }

  int Map::GetX() const { return x; }

  int Map::GetY() const { return y; }

  void Map::RenderToScreen(bool recalculate) const {
    if (active == nullptr) {
      return;
    }

    int xpos = this->x * frame_config_.GetTileDimension();
    int ypos = this->y * frame_config_.GetTileDimension();
    auto screen_w = frame_config_.GetWidth();
    auto screen_h = frame_config_.GetHeight();
    auto screen_start_x = frame_config_.GetOffsetX();
    auto screen_start_y = frame_config_.GetOffsetY();

    SDL_Rect src{xpos, ypos, screen_w, screen_h};
    SDL_Rect dest{screen_start_x, screen_start_y, screen_w, screen_h};

    auto offset_x_start = 0;
    auto offset_x = 0;

    if (x > 0) {
      src.x = 0;
      src.y = ypos;
      src.w = static_cast<int>(this->x * config::TILE_DIMENSIONS);
      src.h = screen_h - ypos;

      dest.x = screen_start_x + (screen_w - xpos);
      dest.y = screen_start_y;
      dest.w = src.w;
      dest.h = src.h;

      offset_x = xpos;

      if (active_map->HasConnection(RIGHT_CONNECTION)) {
        SDL_RenderCopy(
            sdl::renderer,
            active_map->active->connections_.Right->map_texture_.mTexture,
            &src,
            &dest
        );
      } else {
        SDL_RenderCopy(
            sdl::renderer,
            border_map_.map_texture_.mTexture,
            &src,
            &dest
        );
      }
    } else if (x < 0) {
      auto x_abs = abs(x);
      xpos = x_abs;
      src.x = screen_w - (x_abs * config::TILE_DIMENSIONS);
      src.y = ypos;
      src.w = static_cast<int>(x_abs * config::TILE_DIMENSIONS);
      src.h = screen_h - ypos;

      offset_x_start = src.w;
      dest = src;
      dest.x = screen_start_x;
      dest.y = screen_start_y;

      if (active_map->HasConnection(LEFT_CONNECTION)) {
        SDL_RenderCopy(
            sdl::renderer,
            active_map->active->connections_.Left->map_texture_.mTexture,
            &src,
            &dest
        );
      } else {
        SDL_RenderCopy(
            sdl::renderer,
            border_map_.map_texture_.mTexture,
            &src,
            &dest
        );
      }
    }

    auto offset_y = 0;
    if (y > 0) {
      src.x = offset_x;
      src.y = 0;
      src.w = screen_w - offset_x;
      src.h = static_cast<int>(ypos);

      dest.x = screen_start_x + offset_x_start;
      dest.y = screen_start_y + (screen_h - (this->y * config::TILE_DIMENSIONS));
      dest.h = src.h;
      dest.w = src.w;

      offset_y = ypos;

      if (active_map->HasConnection(BOTTOM_CONNECTION)) {
        SDL_RenderCopy(
            sdl::renderer,
            active_map->active->connections_.Down->map_texture_.mTexture,
            &src,
            &dest
        );
      } else {
        SDL_RenderCopy(
            sdl::renderer,
            border_map_.map_texture_.mTexture,
            &src,
            &dest
        );
      }
    }

    src.x = xpos;
    src.y = ypos;
    src.w = screen_w;
    src.h = screen_h;

    dest.x = screen_start_x + offset_x_start;
    dest.y = screen_start_y;
    dest.w = screen_w - offset_x;
    dest.h = screen_h - offset_y;

    SDL_RenderCopy(
        sdl::renderer,
        map::active_map->active->map_texture_.mTexture,
        &src,
        &dest
    );

    // Render border map for bottom left..
    if (offset_x > 0 && offset_y > 0) {
      src.x = 0;
      src.y = 0;
      src.w = offset_x;
      src.h = offset_y;

      dest = src;
      dest.x = screen_start_x + (screen_w - offset_x);
      dest.y = screen_start_y + (screen_h - offset_y);
      SDL_RenderCopy(
          sdl::renderer,
          border_map_.map_texture_.mTexture,
          &src,
          &dest
      );
    }

    if (offset_x_start > 0 && offset_y > 0 ) {
      src.x = screen_w - offset_x_start;
      src.y = 0;
      src.w = offset_x_start;
      src.h = offset_y;

      dest = src;
      dest.x = 0;
      dest.y = screen_start_y +  (screen_h - offset_y);

      SDL_RenderCopy(
          sdl::renderer,
          border_map_.map_texture_.mTexture,
          &src,
          &dest
      );
    }
  }

// TODO: Only supports one block movements for now

  int Map::GetTileFromMouse(int mx, int my) const {
    return 0;
//    mx = (mx - config::SCREEN_OFFSET_X) / config::TILE_DIM;
//    my = (my - config::SCREEN_OFFSET_Y) / config::TILE_DIM;
//    my += y;
//    mx += x;
//
//    if (mx > (int) total_width_ || my > (int) total_height_) {
//      printf("Invalid Mouse X:%d, Y:%d\n", mx, my);
//      return 0;
//    }
//
//    printf("TILE GET X:%d, Y:%d\n", mx, my);
//    return active->GetTile(mx, my);
  }

  void Map::SetTile(int mx, int my, int tile) const {
    mx = (mx - frame_config_.GetOffsetX()) / frame_config_.GetTileDimension();
    my = (my - frame_config_.GetOffsetY()) / frame_config_.GetTileDimension();
    my += y;
    mx += x;

    if (mx > (int) total_width_ || my > (int) total_height_) {
      printf("Invalid Mouse X:%d, Y:%d\n", mx, my);
    }

    active->SetTile(mx, my, tile);
  }

  void Map::SaveMap(size_t index) {
    map_data_list[index].SaveMap();
  }

  void Map::LoadMap(size_t index) {
    map_data_list[index].LoadMap();
  }

  void Map::PrintMap(size_t index) {
    map_data_list[index].PrintMap();
  }

  std::vector<std::string> Map::GetMapConnections() const {
    std::vector<std::string> map_names{};

    for (auto &map_data: map_data_list) {
      if (!map_data.filename_.empty()) {
        map_names.push_back(map_data.filename_);
      }
    }

    return map_names;
  }
} /*namespace map_ */
