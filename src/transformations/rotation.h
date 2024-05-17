#pragma once
#include "transformation.h"

class rotation final : public transformation
{
public:
    static rotation x_rotation(float angle);
    static rotation y_rotation(float angle);
    static rotation z_rotation(float angle);
};
