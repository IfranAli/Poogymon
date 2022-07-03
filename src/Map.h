#pragma once

#include <vector>
#include <string>
#include "MapData.h"
#include "Objects/FrameConfig.h"

namespace map {
  class Map;
  constexpr float transition_speed_default = 10;
  extern Map *active_map;
  extern int pole; // -1 = UP, LEFT
  extern int direction;
  extern bool moving;
  extern float target;
  extern float *axis;
//extern float x;
//extern float y;
  extern float speed;

  void MoveSmooth(int vx, int vy, bool move_camera);
  void MoveSmoothStep();

  enum MapIndex : unsigned int {
    ACTIVE,
    LEFT_CONNECTION,
    RIGHT_CONNECTION,
    TOP_CONNECTION,
    BOTTOM_CONNECTION,
  };

  enum Direction : int {
    UP,
    DOWN,
    LEFT,
    RIGHT,
  };

  struct MapMetaData {
    std::string filename{};
    std::vector<std::string> texture_filenames{};
    map::MapIndex connection_type{};
  };

  class Map final {
   public:

    Map &operator=(Map &&) noexcept;
    explicit Map(const std::string &master_file_name);
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

    map_data::MapData *active{nullptr};
    map_data::MapData map_data_list[MapIndex::BOTTOM_CONNECTION];
    map_data::MapData &GetConnection(MapIndex);
    bool HasConnection(MapIndex);
    void AddMapData(map_data::MapData &map_data);
    void RenderToScreen(FrameConfig, bool recalculate = false) const;

    float x = 0.0;
    float y = 0.0;
    float transition_speed_ = 10;
    [[nodiscard]] int GetTotalWidth() const;
    [[nodiscard]] int GetTotalHeight() const;
    [[nodiscard]] int GetXMax() const;
    [[nodiscard]] int GetXMin() const;
    [[nodiscard]] int GetYMax() const;
    [[nodiscard]] int GetYMin() const;

    int total_width_ = 0;
    int total_height_ = 0;
    int x_max_ = 0;
    int x_min_ = 0;
    int y_max_ = 0;
    int y_min_ = 0;

   private:
    bool hide_rendering_border_ = true;
    bool show_tile_set_ = true;
  };

} /* map */
