#ifndef GAME_H
#define GAME_H

#include "System.h"
#include "Screen.h"

enum class CenterType { CENTER_OF_MASS, AVERAGE_POSITION, BOUNDS };
enum class ScaleType { NO_SCALE, SCALE, MAX_SCALE };

class Game {
  private:
    System sys;
    SoftScreen *scr;
    Linear::Vec2d center;
    double scale; // A scaling factor for drawing in units (meters per pixel)
    double max_scale;
    bool simulate;
    bool running;
    CenterType center_type;
    ScaleType scale_type;
    double time_step;
    int steps_per_frame;
    int selected_body;

    void handle_input();
    void set_scale_variables();
    void draw_system();

  public:
    Game(SoftScreen *scr);
    ~Game();
    void run();
};

#endif
