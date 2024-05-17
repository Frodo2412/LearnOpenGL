#pragma once
#include "vector3.h"

class vector4
{
    float coordinates_[4];

public:
    vector4(const vector3 vector3): coordinates_{vector3.get_x(), vector3.get_y(), vector3.get_z(), 1.0f}
    {
    }
};
