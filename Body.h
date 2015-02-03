#ifndef BODY_H
#define BODY_H

#include <deque>
#include "Vec2.h"

class Body {
    friend class System;
    friend class Game;
    private:
        double mass;
        Vec2 pos;
        Vec2 vel;
        Vec2 force;
        unsigned int trail_length;
        bool movable;
        std::deque<Vec2> trail;


        double speed();
    public:
        Body(double x_pos, double y_pos, double x_vel, double y_vel, double mass, int trail_length = 0, bool movable = true);
        ~Body();

        void flip_movable();
        bool is_movable();
        void register_current_position();
};

#endif
