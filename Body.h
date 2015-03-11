#ifndef BODY_H
#define BODY_H

#include <deque>
#include "Vec.h"

class Body {
    friend class System;
    friend class Game;
    private:
        double mass;
        Linear::Vec2d pos;
        Linear::Vec2d vel;
        Linear::Vec2d force;
        unsigned int trail_length;
        bool movable;
        std::deque<Linear::Vec2d> trail;


        double speed();
    public:
        Body(double x_pos, double y_pos, double x_vel, double y_vel, double mass, int trail_length = 0, bool movable = true);
        ~Body();

        void flip_movable();
        bool is_movable();
        void register_current_position();
};

#endif
