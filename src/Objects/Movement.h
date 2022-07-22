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
  int pole = 0;
  int direction = 0;
  bool moving = false;
  float target = 0;
  float *axis = nullptr;
  float x = 0;
  float y = 0;
  float speed = 32;
};
