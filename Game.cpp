#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include "Game.h"

using namespace std;
using namespace Linear;

const int body_radius = 4;

const double BORDER_SIZE = 0.2;
const int move_factor = 5;

void Game::handle_input() {
  static bool left_pressed = false;
  static bool right_pressed = false;
  static bool up_pressed = false;
  static bool down_pressed = false;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: {
      running = false;
      break;
    }
    case SDL_KEYDOWN: {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE: {
        running = false;
        break;
      }
      case SDLK_SPACE: {
        sys.bodies[selected_body].flip_movable();
        break;
      }
      case SDLK_BACKSPACE: {
        if (sys.num_bodies() == 1) {
          running = false;
        }
        sys.bodies.erase(sys.bodies.begin() + selected_body);
        if (selected_body >= sys.num_bodies()) {
          selected_body = sys.num_bodies() - 1;
        }
        break;
      }
      case SDLK_RETURN: {
        simulate = !simulate;
        break;
      }
      case SDLK_LSHIFT:
      case SDLK_RSHIFT: {
        selected_body++;
        selected_body %= sys.num_bodies();
        break;
      }
      case SDLK_LEFT: {
        left_pressed = true;
        break;
      }
      case SDLK_RIGHT: {
        right_pressed = true;
        break;
      }
      case SDLK_UP: {
        up_pressed = true;
        break;
      }
      case SDLK_DOWN: {
        down_pressed = true;
        break;
      }
      case SDLK_1: {
        time_step /= 2.0;
        break;
      }
      case SDLK_2: {
        time_step *= 2.0;
        break;
      }
      case SDLK_3: {
        scale_option = NO_SCALE;
        break;
      }
      case SDLK_4: {
        scale_option = SCALE;
        break;
      }
      case SDLK_5: {
        scale_option = MAX_SCALE;
        max_scale = scale;
        break;
      }
      case SDLK_6: {
        center_option = AVERAGE_POSITION;
        break;
      }
      case SDLK_7: {
        center_option = CENTER_OF_MASS;
        break;
      }
      case SDLK_8: {
        center_option = BOUNDS;
        break;
      }
      case SDLK_0: {
        scr->toggle_recording();
        break;
      }
      default: { break; }
      }
      break;
    }
    case SDL_KEYUP: {
      switch (event.key.keysym.sym) {
      case SDLK_LEFT: {
        left_pressed = false;
        break;
      }
      case SDLK_RIGHT: {
        right_pressed = false;
        break;
      }
      case SDLK_UP: {
        up_pressed = false;
        break;
      }
      case SDLK_DOWN: {
        down_pressed = false;
        break;
      }
      default: { break; }
      }
    }
    default: { break; }
    }
  }
  bool register_new_position = false;
  if (up_pressed) {
    sys.bodies[selected_body].pos.y += move_factor * scale;
    register_new_position = true;
  }
  if (down_pressed) {
    sys.bodies[selected_body].pos.y -= move_factor * scale;
    register_new_position = true;
  }
  if (left_pressed) {
    sys.bodies[selected_body].pos.x -= move_factor * scale;
    register_new_position = true;
  }
  if (right_pressed) {
    sys.bodies[selected_body].pos.x += move_factor * scale;
    register_new_position = true;
  }
  if (register_new_position) {
    sys.bodies[selected_body].register_current_position();
  }
}

Game::Game(SoftScreen *scr)
    : scr(scr), simulate(false), running(true), center_option(AVERAGE_POSITION),
      scale_option(MAX_SCALE), time_step(50.0), steps_per_frame(100),
      selected_body(0) {
#if 1
  //                     x_pos         y_pos    x_vel    y_vel    mass trail
  sys.add_body(0.0, 0.0, 0.0, 0.0, 6.0E24, 500);
  sys.add_body(400000000.0, 0.0, 0.0, 1006.0, 7.0E22, 500);
  sys.add_body(420000000.0, 0.0, 0.0, 1500.0, 4.0E20, 500);
  sys.add_body(0.0, 400000000.0, -1006.0, 0.0, 7.0E22, 500);
  sys.add_body(0.0, 420000000.0, -1500.0, 0.0, 4.0E20, 500);
  sys.add_body(-400000000.0, 0.0, 0.0, -1006.0, 7.0E22, 500);
  sys.add_body(-420000000.0, 0.0, 0.0, -1500.0, 4.0E20, 500);
  sys.add_body(0.0, -400000000.0, 1006.0, 0.0, 7.0E22, 500);
  sys.add_body(0.0, -420000000.0, 1500.0, 0.0, 4.0E20, 500);
  sys.set_minimum_distance(4000000.0);
#else
  srand(time(0));
  random_device rd;
  mt19937_64 gen(rd());
  uniform_real_distribution<> dis(-100, 100);
  for (int i = 0; i < 100; ++i) {
    double x_pos = dis(gen);
    double y_pos = dis(gen);
    double x_vel = 0.0;
    double y_vel = 0.0;
    x_vel = 0.0;
    y_vel = 0.0;
    double mass = 100.0;
    sys.add_body(x_pos, y_pos, x_vel, y_vel, mass, 1000);
  }
  sys.set_minimum_distance(2.0);
#endif
  set_scale_variables();
  max_scale = scale;
  scr->set_recording_style("images", 5);
}

Game::~Game() {}

void Game::set_scale_variables() {
  double right_bound = sys.bodies[0].pos.x;
  double left_bound = sys.bodies[0].pos.x;
  double up_bound = sys.bodies[0].pos.y;
  double down_bound = sys.bodies[0].pos.y;
  if (center_option == AVERAGE_POSITION) {
    center = sys.bodies[0].pos;
  } else if (center_option == CENTER_OF_MASS) {
    center = (sys.bodies[0].pos * sys.bodies[0].mass);
  }
  double sum_mass = sys.bodies[0].mass;
  for (int i = 1; i < sys.num_bodies(); ++i) {
    if (sys.bodies[i].pos.x > right_bound) {
      right_bound = sys.bodies[i].pos.x;
    }
    if (sys.bodies[i].pos.x < left_bound) {
      left_bound = sys.bodies[i].pos.x;
    }
    if (sys.bodies[i].pos.y > up_bound) {
      up_bound = sys.bodies[i].pos.y;
    }
    if (sys.bodies[i].pos.y < down_bound) {
      down_bound = sys.bodies[i].pos.y;
    }
    if (center_option == CENTER_OF_MASS) {
      center += (sys.bodies[i].pos * sys.bodies[i].mass);
    } else if (center_option == AVERAGE_POSITION) {
      center += sys.bodies[i].pos;
    }
    sum_mass += sys.bodies[i].mass;
  }
  if (right_bound == left_bound) {
    double diff = abs(right_bound) * .05;
    right_bound += diff + 10.0;
    left_bound -= diff + 10.0;
  }
  if (up_bound == down_bound) {
    double diff = abs(up_bound) * .05;
    up_bound += diff + 10.0;
    down_bound -= diff + 10.0;
  }
  switch (center_option) {
  case AVERAGE_POSITION:
    center /= sys.num_bodies();
    break;
  case CENTER_OF_MASS:
    center /= sum_mass;
    break;
  case BOUNDS:
    center.x = (right_bound + left_bound) / 2.0;
    center.y = (up_bound + down_bound) / 2.0;
    break;
  default:
    break;
  }
  double max_x = max(abs(right_bound - center.x), abs(left_bound - center.x));
  double max_y = max(abs(up_bound - center.y), abs(down_bound - center.y));
  double scale_x = max_x * (2.0 + BORDER_SIZE) / scr->width;
  double scale_y = max_y * (2.0 + BORDER_SIZE) / scr->height;
  scale = max(scale_x, scale_y);
  if (scale > max_scale) {
    max_scale = scale;
  }
  if (scale_option == MAX_SCALE) {
    scale = max_scale;
  }
}

void Game::draw_system() {
  scr->cls();
  // Perform Scaling
  if (scale_option != NO_SCALE) {
    set_scale_variables();
  }

  // Find maximum speed
  double max_speed = 0.0;
  sys.bodies[0].speed();
  for (int i = 1; i < sys.num_bodies(); ++i) {
    if (sys.bodies[i].speed() > max_speed) {
      max_speed = sys.bodies[i].speed();
    }
  }

  // Draw trails
  scr->set_color(100, 100, 100);
  for (Body &body : sys.bodies) {
    Vec2d last_point;
    bool first_point_seen = false;
    for (auto &vec : body.trail) {
      if (first_point_seen) {
        scr->draw_line(scr->width / 2 + (vec.x - center.x) / scale,
                       scr->height / 2 - (vec.y - center.y) / scale,
                       scr->width / 2 + (last_point.x - center.x) / scale,
                       scr->height / 2 - (last_point.y - center.y) / scale);
      } else {
        first_point_seen = true;
      }
      last_point = vec;
    }
  }

  // Fill in Bodies
  for (Body &body : sys.bodies) {
    double magnitude = 511.0 * body.speed() / max_speed;
    double blue = 0.0;
    double red;
    double green;
    if (magnitude <= 255.0) {
      red = 255.0;
      green = magnitude;
    } else {
      red = 511.0 - magnitude;
      green = 255.0;
    }

    if (!body.is_movable()) {
      red = 0.0;
      green = 0.0;
      blue = 255.0;
    }

    int x = lrint((body.pos.x - center.x) / scale);
    int y = lrint((body.pos.y - center.y) / scale);

    scr->fill_circle(
        scr->width / 2 + x, scr->height / 2 - y, body_radius,
        {(Uint8)lrint(red), (Uint8)lrint(green), (Uint8)lrint(blue)});
  }

  // Draw Selected Body with blinking crosshair
  int x = lrint((sys.bodies[selected_body].pos.x - center.x) / scale);
  int y = lrint((sys.bodies[selected_body].pos.y - center.y) / scale);
  static int i = 0;
  if (i >= 10) {
    scr->fill_circle(scr->width / 2 + x, scr->height / 2 - y, body_radius + 1,
                     {200, 200, 200});
  }
  if (++i == 20) {
    i = 0;
  }
}

// Currently the selected body is not gauranteed to remiain the same over a
// single
// step of simulation. Change this
void Game::run() {
  while (running) {
    if (simulate) {
      for (int i = 0; i < steps_per_frame; ++i) {
        sys.step(time_step);
        if (selected_body >= sys.num_bodies()) {
          selected_body = sys.num_bodies() - 1;
        }
      }
      for (Body &body : sys.bodies) {
        body.register_current_position();
      }
    }
    draw_system();
    scr->commit();
    handle_input();
  }
}
