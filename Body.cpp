#include "Body.h"

Body::Body(double x_pos, double y_pos, double x_vel, double y_vel, double mass, int trail_length, bool movable):
    mass(mass),
    pos(x_pos, y_pos),
    vel(x_vel, y_vel),
    force(0.0, 0.0),
    trail_length(trail_length),
    movable(movable) {
}

Body::~Body() {
}

double Body::speed()
{
    return vel.magnitude();
}

void Body::flip_movable() {
    movable = !movable;
    vel.x = 0.0;
    vel.y = 0.0;
}

bool Body::is_movable() {
    return movable;
}

void Body::register_current_position() {
    trail.push_back(pos);
    if (trail.size() > trail_length) {
        trail.pop_front();
    }
}

