#include <algorithm>
#include <cmath>
#include "System.h"

using std::vector;
using std::max;

const double UGC = 6.67384E-11;

System::System() {
}

System::~System() {
}

void System::calculate_forces() {
    for (int i = 0; i < num_bodies(); ++i) {
        for (int j = i + 1; j < num_bodies(); ++j) {
            Vec2 delta_pos = bodies[j].pos - bodies[i].pos;
            double theta = delta_pos.angle();
            double force_mag = UGC * bodies[i].mass * bodies[j].mass /
                delta_pos.magnitude_square();
            Vec2 force_vec = Vec2(force_mag * cos(theta),
                                  force_mag * sin(theta));
            bodies[i].force += force_vec;
            bodies[j].force -= force_vec;
        }
    }
}

void System::move_bodies(double time_step) {
    for (Body& body: bodies) {
        if (body.is_movable()) {
            body.vel += body.force * (time_step / body.mass);
            body.pos += body.vel * time_step;
        }
        body.force.zero();
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
    calculate_forces();
    move_bodies(time_step);
}

