#include <algorithm>
#include "Game.h"

using std::max;
using std::min;

const int body_radius = 4;

const double SCALE_DENOM = 2.25;
const int move_factor = 15;

static double abs(double val) {
    if (val < 0) {
        return -val;
    }
    return val;
}

void Game::handle_input() {
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
                sys->bodies[selected_body].flip_movable();
                break;
            }
            case SDLK_BACKSPACE: {
                if (sys->num_bodies() == 1) {
                    running = false;
                }
                sys->bodies.erase(sys->bodies.begin() + selected_body);
                if (selected_body == sys->num_bodies()) {
                    selected_body--;
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
                selected_body %= sys->num_bodies();
                break;
            }
            case SDLK_LEFT: {
                sys->bodies[selected_body].pos.x -= move_factor * scale;
                sys->bodies[selected_body].register_current_position();
                break;
            }
            case SDLK_RIGHT: {
                sys->bodies[selected_body].pos.x += move_factor * scale;
                sys->bodies[selected_body].register_current_position();
                break;
            }
            case SDLK_UP: {
                sys->bodies[selected_body].pos.y += move_factor * scale;
                sys->bodies[selected_body].register_current_position();
                break;
            }
            case SDLK_DOWN: {
                sys->bodies[selected_body].pos.y -= move_factor * scale;
                sys->bodies[selected_body].register_current_position();
                break;
            }
            case SDLK_1: {
                do_scale = !do_scale;
                break;
            }
            case SDLK_2: {
                time_step /= 2.0;
                break;
            }
            case SDLK_3: {
                time_step *= 2.0;
                break;
            }
            default: {
                break;
            }
            }
            break;
        }
        default: {
             break;
        }
        }
    }
}

Game::Game(System* sys, Screen* scr) : 
    sys(sys),
    scr(scr),
    do_scale(true),
    simulate(true),
    running(true),
    time_step(50.0),
    steps_per_frame(100),
    selected_body(0) {
    double max_val = 0.0;
    for (Body& body: sys->bodies) {
        if (abs(body.pos.x) > max_val) {
            max_val = abs(body.pos.x);
        }
        if (abs(body.pos.y) > max_val) {
            max_val = abs(body.pos.y);
        }
    }
    default_scale = max_val / (min(scr->width, scr->height) / SCALE_DENOM);
    scale = default_scale;
}

Game::~Game() {
}

void Game::draw_system() {
    scr->cls();
    // Perform Scaling
    double max_x = 0.0;
    double max_y = 0.0;
    for (Body& body: sys->bodies) {
        if (abs(body.pos.x) > max_x) {
            max_x = abs(body.pos.x);
        }
        if (abs(body.pos.y) > max_y) {
            max_y = abs(body.pos.y);
        }
    }
    double scale_x = max_x / (scr->width / SCALE_DENOM);
    double scale_y = max_y / (scr->height / SCALE_DENOM);
    if (do_scale && max(scale_x, scale_y) > default_scale) {
        scale = max(scale_x, scale_y);
    } else {
        scale = default_scale;
    }

    // Find maximum speed
    double max_speed = 1.0;
    for (Body& body: sys->bodies) {
        double speed = body.speed();
        if (speed > max_speed) {
            max_speed = speed;
        }
    }

    for (Body& body: sys->bodies) {
        // Draw trails
        Vec2 last_point;
        bool first_point_seen = false;
        for (Vec2& vec: body.trail) {
            if (first_point_seen) {
                scr->draw_line(scr->width / 2 + vec.x / scale,
                               scr->height / 2 - vec.y / scale,
                               scr->width / 2 + last_point.x / scale,
                               scr->height / 2 - last_point.y / scale,
                               {100, 100, 100});
            } else {
                first_point_seen = true;
            }
            last_point = vec;
        }

        // Fill in Bodies
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

        int x = lrint(body.pos.x / scale);
        int y = lrint(body.pos.y / scale);

        scr->fill_circle(scr->width / 2 + x, scr->height / 2 - y, body_radius,
                         {(Uint8)lrint(red), (Uint8)lrint(green), (Uint8)lrint(blue)});
    }

    // Draw Selected Body with blinking crosshair
    int x = lrint(sys->bodies[selected_body].pos.x / scale);
    int y = lrint(sys->bodies[selected_body].pos.y / scale);
    static int i = 0;
    if (i >= 10) {
        scr->fill_circle(scr->width / 2 + x, scr->height / 2 - y, body_radius + 1,
                         {200, 200, 200});
    }
    if (++i == 20) {
        i = 0;
    }
}

void Game::run() {
    while (running) {
        if (simulate) {
            for (int i = 0; i < steps_per_frame; ++i) {
                sys->step(time_step);
            }
            for (Body& body: sys->bodies) {
                body.register_current_position();
            }
        }
        draw_system();
        scr->commit();
        handle_input();
    }
}

