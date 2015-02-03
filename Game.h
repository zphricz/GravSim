#ifndef GAME_H
#define GAME_H

#include "System.h"
#include "Screen.h"

class Game {
    private:
        System* sys;
        Screen* scr;
        double scale;
        double default_scale;
        bool do_scale;
        bool simulate;
        bool running;
        double time_step;
        int steps_per_frame;
        int selected_body;

        void handle_input();
        void draw_system();
    public:
        Game(System* sys, Screen* scr);
        ~Game();
        void run();
};

#endif
