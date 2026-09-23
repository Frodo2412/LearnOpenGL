#ifndef LEARNOPENGL_CLOCK_H
#define LEARNOPENGL_CLOCK_H


class Clock {
    static float current_frame, last_frame, delta_time;

public:
    static float get_elapsed_time();
};


#endif //LEARNOPENGL_CLOCK_H
