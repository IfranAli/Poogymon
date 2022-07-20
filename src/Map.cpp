#include "Map.h"
#include "Renderer.h"
#include "utility/Utility.h"

#include <SDL.h>
#include <iostream>
#include <sstream>
#include <utility>

namespace map {
  int pole{0}; // -1 = UP, LEFT
  int direction = 0;
  Direction direction_moving = UP;
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
    return ((y_ - 1) >= y_min_);
  }

  bool Map::CanMoveDown() const {
    auto result = ((y_ + frame_config_.GetRows() + 1) <= y_max_);
    return result;
  }

  bool Map::CanMoveLeft() const {
    auto result = (x_ - 1) > x_min_;
    return result;
  }

  bool Map::CanMoveRight() const {
    return ((x_ + frame_config_.GetCols() + 1) <= x_max_);
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
      axis = (vx ? &map::active_map->x_ : &map::active_map->y_);
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
    map_data::TilePattern tile_pattern_2(2, 2, 1, 1);
    map_data::TilePattern tile_pattern_3(2);
    map_data::TilePattern tile_pattern_4(8);
    map_data::TilePattern tile_pattern_5(1);

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

  int Map::GetX() const { return x_; }

  int Map::GetY() const { return y_; }

  void Map::RenderToScreen(bool recalculate) const {
    if (active == nullptr) {
      return;
    }

    auto x = x_ * config::TILE_DIMENSIONS;
    auto y = y_ * config::TILE_DIMENSIONS;
    auto w = frame_config_.GetWidth();
    auto h = frame_config_.GetHeight();

    if (x < 0) {
      x = (x * -1);
      w -= x;
    } else {
      if (x > 0) {
        w -= x;
        x = 0;
      }
    }

    auto gap_left = x;
    auto gap_right = frame_config_.GetWidth() - w;
    auto gap_top = 0;
    auto gap_bottom = 0;

    if (y < 0) {
      gap_top = (y * -1);
      y = gap_top;
      h -= gap_top;
    } else {
      gap_bottom = y;
    }

    if (gap_left > 0) {
      gap_right = 0;
    }

    if (gap_top > 0) {
      if (gap_left > 0) {
        RenderTop(x, 0, w, y, false, true);
        RenderActive(x, y, w, h, false, false);
        RenderBorder(0, 0, gap_left, gap_top, true, true);

      } else {
        RenderTop(x, 0, w, y, true, true);
        RenderActive(x, y, w, h, true, false);

        if (gap_right > 0) {
          RenderBorder(w, 0, gap_right, gap_top, false, true);
        }
      }

    } else {
      if (gap_bottom > 0) {
        h -= gap_bottom;
        if (gap_right > 0) {
          RenderBottom(0, h, w, gap_bottom, true, false);
          RenderActive(x, 0, w, h, true, true);
          RenderBorder(w, h, gap_right, gap_bottom, false, false);
        } else {
          RenderBottom(x, h, w, gap_bottom, false, false);
          RenderActive(x, 0, w, h, false, true);

          if (gap_left > 0) {
            RenderBorder(0, h, gap_left, gap_bottom, true, false);
          }
        }
      } else {
        if (gap_right > 0) {
          RenderActive(x, y, w, h, true, true);
        } else {
          RenderActive(x, y, w, h, false, false);
        }
      }
    }

    h = frame_config_.GetHeight() - gap_bottom;
    if (gap_left > 0) {
      x = 0;
      y = gap_top;
      w = gap_left;

      if (gap_bottom > 0) {
        RenderLeft(x, y, w, h, true, true);
      } else {
        RenderLeft(x, y, w, h, true, false);
      }

    } else {
      if (gap_right > 0) {
        x = w;
        w = gap_right;

        if (gap_bottom > 0) {
          y = 0;
          RenderRight(x, y, w, h, false, true);
        } else {
          RenderRight(x, y, w, h, false, false);
        }
      }
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
    my += y_;
    mx += x_;

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

  void Map::RenderActive(float x,
                         float y,
                         int w,
                         int h,
                         bool draw_from_bottom = false,
                         bool draw_from_top = false) const {
    // Render centre map.
    SDL_Rect src;

    src.x = 0;
    src.y = 0;

    if (draw_from_bottom) {
      src.x = frame_config_.GetWidth() - w;
    }

    if (draw_from_top) {
      src.y = frame_config_.GetHeight() - h;
    }

    src.w = w;
    src.h = h;

    SDL_Rect dest = src;
    dest.x = frame_config_.GetOffsetX() + x;
    dest.y = frame_config_.GetOffsetY() + y;

    SDL_RenderCopy(
        sdl::renderer,
        map::active_map->active->map_texture_.mTexture,
        &src,
        &dest
    );
  }
  void Map::RenderLeft(float x,
                       float y,
                       int w,
                       int h,
                       bool draw_from_bottom = false,
                       bool draw_from_top = false) const {
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = w;
    src.h = h;

    if (draw_from_bottom) {
      src.x = frame_config_.GetWidth() - w;
    }

    if (draw_from_top) {
      src.y = frame_config_.GetHeight() - h;
    }

    SDL_Rect dest = src;
    dest.x = frame_config_.GetOffsetX() + x;
    dest.y = frame_config_.GetOffsetY() + y;

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

  void Map::RenderRight(float x,
                        float y,
                        int w,
                        int h,
                        bool draw_from_bottom = false,
                        bool draw_from_top = false) const {
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = w;
    src.h = h;

    if (draw_from_bottom) {
      src.x = frame_config_.GetWidth() - w;
    }

    if (draw_from_top) {
      src.y = frame_config_.GetHeight() - h;
    }

    SDL_Rect dest = src;
    dest.x = frame_config_.GetOffsetX() + x;
    dest.y = frame_config_.GetOffsetY() + y;

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
  }
  void Map::RenderBottom(float x,
                         float y,
                         int w,
                         int h,
                         bool draw_from_bottom = false,
                         bool draw_from_top = false) const {
    SDL_Rect src = {0, 0, 0, 0};
    src.x = 0;
    src.y = 0;

    if (draw_from_bottom) {
      src.x = frame_config_.GetWidth() - w;
    }

    if (draw_from_top) {
      src.y = frame_config_.GetHeight() - h;
    }

    src.w = w;
    src.h = h;

    SDL_Rect dest = src;
    dest.x = frame_config_.GetOffsetX() + x;
    dest.y = frame_config_.GetOffsetY() + y;

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

  void Map::RenderTop(float x, float y, int w, int h, bool draw_from_bottom = false, bool draw_from_top = false) const {
    SDL_Rect src = {0, 0, 0, 0};
    src.x = 0;
    src.y = 0;

    if (draw_from_bottom) {
      src.x = frame_config_.GetWidth() - w;
    }

    if (draw_from_top) {
      src.y = frame_config_.GetHeight() - h;
    }

    src.w = w;
    src.h = h;

    SDL_Rect dest = src;
    dest.x = frame_config_.GetOffsetX() + x;
    dest.y = frame_config_.GetOffsetY() + y;

    SDL_RenderCopy(
        sdl::renderer,
        active_map->active->connections_.Up->map_texture_.mTexture,
        &src,
        &dest
    );
  }

  void Map::RenderBorder(float x,
                         float y,
                         int w,
                         int h,
                         bool draw_from_bottom = false,
                         bool draw_from_top = false) const {
    SDL_Rect src = {0, 0, 0, 0};
    src.x = 0;
    src.y = 0;

    if (draw_from_bottom) {
      src.x = frame_config_.GetWidth() - w;
    }

    if (draw_from_top) {
      src.y = frame_config_.GetHeight() - h;
    }

    src.w = w;
    src.h = h;

    SDL_Rect dest = src;
    dest.x = frame_config_.GetOffsetX() + x;
    dest.y = frame_config_.GetOffsetY() + y;

    SDL_RenderCopy(
        sdl::renderer,
        border_map_.map_texture_.mTexture,
        &src,
        &dest
    );
  }

} /*namespace map_ */
