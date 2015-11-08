#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "Screen.h"
#include "Body.h"

class System {
  friend class Game;

private:
  std::vector<Body> bodies;
  double minimum_distance;

  void calculate_forces(double time_step);
  void move_bodies(double time_step);

public:
  System();
  ~System();

  void add_body(Body &body);
  void add_body(Body &&body);
  void add_body(double x_pos, double y_pos, double x_vel, double y_vel,
                double mass, int track_length = 0, bool moveable = true);

  int num_bodies();
  void step(double time_step);
  void set_minimum_distance(double distance);
};

#endif
