#include "Screen.h"
#include "System.h"
#include "Body.h"
#include "Game.h"

static void error(char * name) {
    printf("Usage: %s [Screen_x Screen_y]\n", name);
    exit(1);
}

int main(int argc, char* argv[])
{
    bool default_screen;
    int screen_width;
    int screen_height;
    SDL_DisplayMode display;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        exit(1);
    }
    SDL_GetCurrentDisplayMode(0, &display);
    atexit(SDL_Quit);

    switch (argc) {
    case 1:
        default_screen = true;
        break;
    case 3:
        screen_width = atoi(argv[1]);
        screen_height = atoi(argv[2]);
        default_screen = false;
        if (screen_width <= 0 || screen_height <= 0) {
            error(argv[0]);
        }
        break;
    default:
        error(argv[0]);
    }

    if (default_screen) {
        screen_width = display.w;
        screen_height = display.h;
    }

    bool full_screen;
    if (screen_width == display.w && screen_height == display.h) {
        full_screen = true;
    } else {
        full_screen = false;
    }

    Screen scr(screen_width, screen_height, full_screen, "Grav Sim", true);
    System sys;
    Body star(0.0, 0.0, 0.0, 0.0, 6.0E24, 0, false);
    Body planet1(400000000.0, 0.0, 0.0, 1006.0, 7.0E22, 150);
    Body moon1(420000000.0, 0.0, 0.0, 1500.0, 4.0E20, 150);
    Body planet2(0.0, 400000000.0, -1006.0, 0.0, 7.0E22, 150);
    Body moon2(0.0, 420000000.0, -1500.0, 0.0, 4.0E20, 150);
    Body planet3(-400000000.0, 0.0, 0.0, -1006.0, 7.0E22, 150);
    Body moon3(-420000000.0, 0.0, 0.0, -1500.0, 4.0E20, 150);
    Body planet4(0.0, -400000000.0, 1006.0, 0.0, 7.0E22, 150);
    Body moon4(0.0, -420000000.0, 1500.0, 0.0, 4.0E20, 150);

    sys.add_body(star);
    sys.add_body(planet1);
    sys.add_body(moon1);
    sys.add_body(planet2);
    sys.add_body(moon2);
    sys.add_body(planet3);
    sys.add_body(moon3);
    sys.add_body(planet4);
    sys.add_body(moon4);

    Game g(&sys, &scr);
    g.run();
    return 0;
}
