//
// Created by Bruno Lemus on 18/9/26.
//

#include "Clock.h"

#include "GLFW/glfw3.h"

float Clock::current_frame = 0;
float Clock::last_frame = 0;
float Clock::delta_time = 0;

float Clock::get_elapsed_time() {
    current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    return delta_time;
}
