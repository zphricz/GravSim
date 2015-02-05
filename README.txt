                     
                         2D GRAVITATIONAL N-BODY SIM

Overview
--------

This program simulates gravitational bodies in orbit on a 2D plane. It uses
Euler's method to approximate the movement of the bodies. To run the program,
simply run:

    ./gravsim

This will run the program in full screen. You can specify the resolution to
render at with command line arguments:

    ./gravsim [Screen_x Screen_y]

You can interact with the simulation by pressing keys on your keyboard:

    1: Halve the timestep
    2: Double the timestep
    3: Turn scaling off
    4: Turn scaling on (will shrink window)
    5: Turn scaling on (will not shrink window)
    6: Toggle centering based on the center of mass of all bodies, and the
       average position of all bodies
    Shift: Switch selected body
    Enter: Toggle between simulating the program and not
    Escape: Stop the program
    Up: Move selected body up
    Down: Move selected body down
    Left: Move selected body left
    Right: Move selected body right
    Space: Toggle selected body's freeze state

To close the program, press the escape key.

Authors
-------

Zack Hricz

