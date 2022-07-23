#pragma once

#include <vector>
#include <string>
#include "MapData.h"
#include "../Objects/FrameConfig.h"
#include "../interfaces/Drawable.h"
#include "Definitions.h"

namespace map {

//  Map *active_map = nullptr;

  class Map : public Drawable {
   public:
    explicit Map(FrameConfig &frame_config);
    ~Map();

    void Tick() const;
    [[nodiscard]] int GetX() const;
    [[nodiscard]] int GetY() const;

    [[nodiscard]] bool CanMoveUp() const;
    [[nodiscard]] bool CanMoveDown() const;
    [[nodiscard]] bool CanMoveLeft() const;
    [[nodiscard]] bool CanMoveRight() const;

    void SetTile(int x, int y, int tile) const;
    [[nodiscard]] int GetTileFromMouse(int x, int y) const;

    void SaveMap(size_t index);
    void LoadMap(size_t index);
    void PrintMap(size_t index);

    [[nodiscard]] std::vector<std::string> GetMapConnections() const;

    map_data::MapData &GetConnection(MapIndex);
    bool HasConnection(MapIndex);
    void AddMapData(map_data::MapData &map_data);
    void RenderToScreen(bool recalculate = false) const;
    map_data::MapData &GetActiveMapData();

    float x_ = 0.0;
    float y_ = 0.0;

   private:
    FrameConfig &frame_config_;

    map_data::TilePattern border_tile_pattern_ = map_data::TilePattern(4, 5, 12, 13);
    map_data::MapData border_map_ = map_data::MapData("tile.png", border_tile_pattern_);

    map_data::MapData *active_ = nullptr;
    map_data::MapData map_data_list_[5];

    bool show_tile_set_ = true;
    int total_width_ = 0;
    int total_height_ = 0;
    int x_max_ = 0;
    int x_min_ = 0;
    int y_max_ = 0;
    int y_min_ = 0;

    void RenderActive(float x, float y, int w, int h, bool, bool) const;
    void RenderLeft(float x, float y, int w, int h, bool, bool) const;
    void RenderRight(float x, float y, int w, int h, bool, bool) const;
    void RenderBottom(float x, float y, int w, int h, bool, bool) const;
    void RenderTop(float x, float y, int w, int h, bool, bool) const;
    void RenderBorder(float x, float y, int w, int h, bool, bool) const;
    void Render() override;
  };

} /* map */
