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
      case ACTIVE:
        break;
      case LEFT_CONNECTION:
        return *map_data_list[MapIndex::ACTIVE].connections.Left;
        break;
      case RIGHT_CONNECTION:
        return *map_data_list[MapIndex::ACTIVE].connections.Right;
        break;
      case TOP_CONNECTION:
        return *map_data_list[MapIndex::ACTIVE].connections.Up;
        break;
      case BOTTOM_CONNECTION:
        return *map_data_list[MapIndex::ACTIVE].connections.Down;
        break;
    }
  }

  bool Map::CanMoveUp() const {
    return ((y - 1) >= y_min_);
  }

  bool Map::CanMoveDown() const {
    auto result = ((y + config::TILE_PER_ROW + 1) <= y_max_);
    return result;
  }

  bool Map::CanMoveLeft() const {
    return (x - 1 >= x_min_);
  }

  bool Map::CanMoveRight() const {
    return ((x + config::TILE_PER_COL + 1) <= x_max_);
  }

  bool CanMove(Direction move_direction) {
    switch (move_direction) {
      case UP: return active_map->CanMoveUp();
      case DOWN:return active_map->CanMoveDown();
      case LEFT:return active_map->CanMoveLeft();
      case RIGHT:return active_map->CanMoveRight();
    }
  }

  void MoveSmooth(Direction move_direction, bool move_camera) {
    if (moving) {
      return;
    }

    auto is_moving_x_axis = (move_direction == Direction::LEFT || move_direction == Direction::RIGHT);
    direction = move_direction;
    pole = move_direction;

    if (CanMove(move_direction)) {
      if (move_camera) {
        axis = (is_moving_x_axis ? &map::active_map->x : &map::active_map->y);
      } else {
        axis = (is_moving_x_axis ? &x : &y);
      }

//      switch (move_direction) {
//        case UP:break;
//        case DOWN:break;
//        case LEFT:break;
//        case RIGHT:break;
//      }

      target = *axis + (direction);
      *axis += (direction * speed) * sdl::deltatime;
      moving = true;
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

  Map::Map(const std::string &master_file_name) {
    auto files = ParseMapFile(master_file_name);

    auto file = files.front();
    map_data_list[MapIndex::ACTIVE] = map_data::MapData(file.filename, file.texture_filenames.front());
//    for (const auto &file: files) {
//      map_data_list[file.connection_type] = map_data::MapData(file.filename, file.texture_filenames.front());
//      switch (file.connection_type) {
//        case ACTIVE:break;
////        case LEFT_CONNECTION:map_data_list[MapIndex::ACTIVE].connections.Left = &map_data_list[file.connection_type];
//          break;
////        case RIGHT_CONNECTION:map_data_list[MapIndex::ACTIVE].connections.Right = &map_data_list[file.connection_type];
//          break;
////        case TOP_CONNECTION:map_data_list[MapIndex::ACTIVE].connections.Up = &map_data_list[file.connection_type];
//          break;
////        case BOTTOM_CONNECTION:map_data_list[MapIndex::ACTIVE].connections.Down = &map_data_list[file.connection_type];
//          break;
//      }
//    }

    AddMapData(map_data_list[MapIndex::ACTIVE]);

    moving = false;
  }

  Map::~Map() {
    printf("MAP -dtor\n");
  }

  Map &Map::operator=(Map &&t) noexcept {
    return *this;
  }

  bool Map::HasConnection(MapIndex type) {
    auto &connection{GetConnection(type)};
    return !connection.filename.empty();
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
    this->total_width_ += mapdata.map_width;
    this->total_height_ += mapdata.map_height;
    this->x_max_ += total_width_;
    this->y_max_ += total_height_;

    if (active->connections.Right != nullptr) {
      auto width = active->connections.Right->map_width;
      this->total_width_ += width;
      this->x_max_ += width;
    }
    if (active->connections.Left != nullptr) {
      auto width = active->connections.Left->map_width;
      this->total_width_ += width;
      this->x_min_ -= width;
    }
    if (active->connections.Up != nullptr) {
      auto width = active->connections.Up->map_width;
      this->total_height_ += width;
      this->y_max_ += width;
    }
    if (active->connections.Down != nullptr) {
      auto width = active->connections.Down->map_width;
      this->total_height_ += width;
      this->y_min_ -= width;
    }

    printf("Total Width: %d\nTotal Height: %d\n", this->total_width_, this->total_height_);
  }

  int Map::GetX() const { return x; }

  int Map::GetY() const { return y; }

  void Map::RenderToScreen(FrameConfig frame_config, bool recalculate) const {
    if (active == nullptr) {
      return;
    }

    int xpos = this->x * config::TILE_DIM;
    int ypos = this->y * config::TILE_DIM;
    auto screen_w = config::TILE_DIM * 15;
    auto screen_h = config::TILE_DIM * 10;
//    SDL_Rect src{xpos, ypos, screen_w, screen_h};
//    SDL_Rect dest{0, 0, screen_w, screen_h};
    SDL_Rect src{xpos, ypos, screen_w, screen_h};
    SDL_Rect dest{0, 0, screen_w, screen_h};

//    config::RecalculateWindowVariables(screen_w, screen_h);

    SDL_RenderCopy(
        sdl::renderer,
        map::active_map->active->map_texture.mTexture,
        &src,
        &dest
    );
  }

// TODO: Only supports one block movements for now

  int Map::GetTileFromMouse(int mx, int my) const {
    mx = (mx - config::SCREEN_OFFSET_X) / config::TILE_DIM;
    my = (my - config::SCREEN_OFFSET_Y) / config::TILE_DIM;
    my += y;
    mx += x;

    if (mx > (int) total_width_ || my > (int) total_height_) {
      printf("Invalid Mouse X:%d, Y:%d\n", mx, my);
      return 0;
    }

    printf("TILE GET X:%d, Y:%d\n", mx, my);
    return active->GetTile(mx, my);
  }

  void Map::SetTile(int mx, int my, int tile) const {
    mx = (mx - config::SCREEN_OFFSET_X) / config::TILE_DIM;
    my = (my - config::SCREEN_OFFSET_Y) / config::TILE_DIM;
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
      if (!map_data.filename.empty()) {
        map_names.push_back(map_data.filename);
      }
    }

    return map_names;
  }
  int Map::GetTotalWidth() const {
    return total_width_;
  }
  int Map::GetTotalHeight() const {
    return total_height_;
  }
  int Map::GetXMax() const {
    return x_max_;
  }
  int Map::GetXMin() const {
    return x_min_;
  }
  int Map::GetYMax() const {
    return y_max_;
  }
  int Map::GetYMin() const {
    return y_min_;
  }
} /*namespace map_ */
