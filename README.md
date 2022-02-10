# TINACalculator
A simulated implementation of a floating-point calculator with hex keypad input and LCD display output, using C programming and TINACloud. It was designed specifically for a TM4C123GH6PM microcontroller and HD44780U LCD display.

The calculation of the input equation is done using a recursive algorithm, breaking the
equation into parts with decreasing order of operator precedence (a rough visual explanation
is also provided in calc_bidmas_logic.txt). The user can also delete the last-entered character
and clear the equation. A shift function has been implemented, to allow all these functions to
be chosen on the keypad by the user. The calculator is password-protected (password is
‘1234’). The program uses C library math.h methods.

This was a project for my final year of study in 2021 at the University of Leeds.

![forcalc](https://user-images.githubusercontent.com/90904328/153440105-cd75860f-3e56-48e2-b64a-4757d3d3781a.png)

*Created by Daniel Burgoyne*
