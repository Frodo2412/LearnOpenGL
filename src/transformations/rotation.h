#pragma once
#include "transformation.h"

class rotation final : public transformation
{
    explicit rotation(const matrix4& matrix): transformation(matrix)
    {
    }

public:
    static rotation x_rotation(float angle);
    static rotation y_rotation(float angle);
    static rotation z_rotation(float angle);
};
