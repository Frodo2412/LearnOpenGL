#include "rotation.h"

#include <cmath>

rotation rotation::x_rotation(float angle)
{
    return transformation({
        1, 0, 0, 0,
        0, cos(angle), -sin(angle), 0,
        0, sin(angle), cos(angle), 0,
        0, 0, 0, 1
    });
}

rotation rotation::y_rotation(float angle)
{
}

rotation rotation::z_rotation(float angle)
{
}
