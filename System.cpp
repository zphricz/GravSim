#include <algorithm>
#include <cmath>
#include "System.h"

using std::vector;
using std::max;

const double UGC = 6.67384E-11;

System::System() :
    minimum_distance(0.0) {
}

System::~System() {
}

void System::calculate_forces(double time_step) {
    for (int i = 0; i < num_bodies(); ++i) {
        for (int j = i + 1; j < num_bodies(); ++j) {
            Vec2 delta_pos = bodies[j].pos - bodies[i].pos;
            double theta = delta_pos.angle();
            double force_mag = UGC * bodies[i].mass * bodies[j].mass /
                delta_pos.magnitude_square();
            Vec2 force_vec(force_mag * cos(theta),
                                  force_mag * sin(theta));
            bodies[i].force += force_vec;
            bodies[j].force -= force_vec;
        }
    }
}

void System::move_bodies(double time_step) {
    for (Body& body: bodies) {
        if (body.is_movable()) {
            body.vel += (body.force * (time_step / body.mass));
            body.pos += (body.vel * time_step);
        }
        body.force.zero();
    }

    // Detect collisions
    for (int i = 0; i < num_bodies(); ++i) {
        for (int j = i + 1; j < num_bodies(); ++j) {
            Vec2 delta_pos = bodies[j].pos - bodies[i].pos;
            if (delta_pos.magnitude() < minimum_distance) {
                Vec2 momentum = (bodies[j].vel * bodies[j].mass) + (bodies[i].vel * bodies[i].mass);
                Vec2 velocity = momentum / (bodies[i].mass + bodies[j].mass);
                if (bodies[i].mass > bodies[j].mass) {
                    bodies[i].mass += bodies[j].mass;
                    bodies[i].vel = velocity;
                    bodies.erase(bodies.begin() + j);
                    j -= 1;
                } else {
                    bodies[j].mass += bodies[i].mass;
                    bodies[j].vel = velocity;
                    bodies.erase(bodies.begin() + i);
                    i -= 1;
                    break;
                }
            }
        }
    }
}

void System::add_body(Body& body) {
    bodies.push_back(body);
}

void System::add_body(Body&& body) {
    bodies.push_back(body);
}

void System::add_body(double x_pos, double y_pos, double x_vel, double y_vel, double mass, int track_length, bool movable) {
    add_body(Body(x_pos, y_pos, x_vel, y_vel, mass, track_length, movable));
}

int System::num_bodies() {
    return bodies.size();
}

void System::step(double time_step) {
    calculate_forces(time_step);
    move_bodies(time_step);
}

void System::set_minimum_distance(double distance) {
    minimum_distance = distance;
}

