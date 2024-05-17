#pragma once
#include "transformation.h"
#include "../geometry/vector3.h"

class translation final : public transformation
{
public:
    explicit translation(const vector3 vector3): transformation(matrix4{
        1, 0, 0, vector3.get_x(),
        0, 1, 0, vector3.get_y(),
        0, 0, 1, vector3.get_z(),
        0, 0, 0, 1
    })
    {
    }
};
