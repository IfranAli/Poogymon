class Movement {

 public:
  Movement();
  ~Movement();
  void Tick();
  void Step();

  void MoveSmooth(int vx, int vy, bool move_camera = false);
  /**
   * 0    : NONE
   * 1    : LEFT, RIGHT
   * -1   : UP, DOWN
   */
  int pole_ = 0;
  int direction_ = 0;
  bool moving_ = false;
  float target_ = 0;
  float *axis_ = nullptr;
  float x_ = 0;
  float y_ = 0;
  float speed_ = 32;
};
