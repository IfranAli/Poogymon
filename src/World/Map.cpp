#include "Map.h"
#include "../Renderer.h"

#include <SDL.h>
#include <utility>

namespace map {
  Map *active_map;

  Map::Map(FrameConfig &frame_config) : frame_config_(frame_config) {
    map_data::TilePattern tile_pattern_1(0);
    map_data::TilePattern tile_pattern_2(2, 2, 1, 1);
    map_data::TilePattern tile_pattern_3(2);
    map_data::TilePattern tile_pattern_4(8);
    map_data::TilePattern tile_pattern_5(1);

    map_data_list_[MapIndex::ACTIVE] = map_data::MapData("tile.png", tile_pattern_1);
    map_data_list_[MapIndex::RIGHT_CONNECTION] = map_data::MapData("tile.png", tile_pattern_2);
    map_data_list_[MapIndex::LEFT_CONNECTION] = map_data::MapData("tile.png", tile_pattern_3);
    map_data_list_[MapIndex::BOTTOM_CONNECTION] = map_data::MapData("tile.png", tile_pattern_4);
    map_data_list_[MapIndex::TOP_CONNECTION] = map_data::MapData("tile.png", tile_pattern_5);

    map_data_list_[ACTIVE].connections_.Right = &map_data_list_[RIGHT_CONNECTION];
    map_data_list_[ACTIVE].connections_.Left = &map_data_list_[LEFT_CONNECTION];
    map_data_list_[ACTIVE].connections_.Down = &map_data_list_[BOTTOM_CONNECTION];
    map_data_list_[ACTIVE].connections_.Up = &map_data_list_[TOP_CONNECTION];

    AddMapData(map_data_list_[ACTIVE]);
  }

  Map::~Map() {
    printf("MAP -dtor\n");
  }

  void Map::Render() {
    RenderToScreen(true);
  }

  map_data::MapData &Map::GetConnection(MapIndex type) {
    switch (type) {
      case ACTIVE:return map_data_list_[ACTIVE];
        break;
      case LEFT_CONNECTION:return *map_data_list_[MapIndex::ACTIVE].connections_.Left;
        break;
      case RIGHT_CONNECTION:return *map_data_list_[MapIndex::ACTIVE].connections_.Right;
        break;
      case TOP_CONNECTION:return *map_data_list_[MapIndex::ACTIVE].connections_.Up;
        break;
      case BOTTOM_CONNECTION:return *map_data_list_[MapIndex::ACTIVE].connections_.Down;
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

  bool Map::HasConnection(MapIndex type) {
    return map_data_list_[type].IsValidMap();
  }

  void Map::Tick() const {

  }

  void Map::AddMapData(map_data::MapData &mapdata) {
    active_ = &mapdata;
    this->total_width_ += mapdata.GetMapWidth();
    this->total_height_ += mapdata.GetMapHeight();
    this->x_max_ += total_width_;
    this->y_max_ += total_height_;

    if (active_->connections_.Right != nullptr) {
      auto width = active_->connections_.Right->GetMapWidth();
      this->total_width_ += width;
      this->x_max_ += width;
    }
    if (active_->connections_.Left != nullptr) {
      auto width = active_->connections_.Left->GetMapWidth();
      this->total_width_ += width;
      this->x_min_ -= width;
    }
    if (active_->connections_.Up != nullptr) {
      auto width = active_->connections_.Up->GetMapWidth();
      this->total_height_ += width;
      this->y_min_ -= width;
    }
    if (active_->connections_.Down != nullptr) {
      auto height = active_->connections_.Down->GetMapHeight();
      this->total_height_ += height;
      this->y_max_ += height;
    }

    printf("Total Width: %d\nTotal Height: %d\n", this->total_width_, this->total_height_);
  }

  int Map::GetX() const { return x_; }

  int Map::GetY() const { return y_; }

  void Map::RenderToScreen(bool  /*recalculate*/) const {
    if (active_ == nullptr) {
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

  int Map::GetTileFromMouse(int mx, int my) const {
    return 0;
//    mx = (mx - config::SCREEN_OFFSET_X) / config::TILE_DIM;
//    my = (my - config::SCREEN_OFFSET_Y) / config::TILE_DIM;
//    my += y_;
//    mx += x_;
//
//    if (mx > (int) total_width_ || my > (int) total_height_) {
//      printf("Invalid Mouse X:%d, Y:%d\n", mx, my);
//      return 0;
//    }
//
//    printf("TILE GET X:%d, Y:%d\n", mx, my);
//    return active_->GetTile(mx, my);
  }

  void Map::SetTile(int mx, int my, int tile) const {
    mx = (mx - frame_config_.GetOffsetX()) / frame_config_.GetTileDimension();
    my = (my - frame_config_.GetOffsetY()) / frame_config_.GetTileDimension();
    my += y_;
    mx += x_;

    if (mx > (int) total_width_ || my > (int) total_height_) {
      printf("Invalid Mouse X:%d, Y:%d\n", mx, my);
    }

    active_->SetTile(mx, my, tile);
  }

  void Map::SaveMap(size_t index) {
    map_data_list_[index].SaveMap();
  }

  void Map::LoadMap(size_t index) {
    map_data_list_[index].LoadMap();
  }

  void Map::PrintMap(size_t index) {
    map_data_list_[index].PrintMap();
  }

  std::vector<std::string> Map::GetMapConnections() const {
    std::vector<std::string> map_names{};

    for (auto &map_data: map_data_list_) {
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

    SDL_RenderCopy(sdl::renderer, map::active_map->active_->map_texture_.mTexture, &src, &dest);
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

    if (active_map->HasConnection(LEFT_CONNECTION) && active_map->active_->connections_.Left) {
      SDL_RenderCopy(sdl::renderer, active_map->active_->connections_.Left->map_texture_.mTexture, &src, &dest);
    } else {
      SDL_RenderCopy(sdl::renderer, border_map_.map_texture_.mTexture, &src, &dest);
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

    if (active_map->HasConnection(RIGHT_CONNECTION) && active_map->active_->connections_.Right) {
      SDL_RenderCopy(sdl::renderer, active_map->active_->connections_.Right->map_texture_.mTexture, &src, &dest);
    } else {
      SDL_RenderCopy(sdl::renderer, border_map_.map_texture_.mTexture, &src, &dest);
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

    if (active_map->HasConnection(BOTTOM_CONNECTION) && active_map->active_->connections_.Down) {
      SDL_RenderCopy(sdl::renderer, active_map->active_->connections_.Down->map_texture_.mTexture, &src, &dest);
    } else {
      SDL_RenderCopy(sdl::renderer, border_map_.map_texture_.mTexture, &src, &dest);
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

    if (active_map->HasConnection(TOP_CONNECTION) && active_map->active_->connections_.Up) {
      SDL_RenderCopy(sdl::renderer, active_map->active_->connections_.Up->map_texture_.mTexture, &src, &dest);
    } else {
      SDL_RenderCopy(sdl::renderer, border_map_.map_texture_.mTexture, &src, &dest);
    }
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

    SDL_RenderCopy(sdl::renderer, border_map_.map_texture_.mTexture, &src, &dest);
  }

  map_data::MapData &Map::GetActiveMapData() {
    return (this->active_ == nullptr) ? map_data_list_[ACTIVE] : *active_;
  }

} /*namespace map_ */