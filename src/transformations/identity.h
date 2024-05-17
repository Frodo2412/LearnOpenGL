#pragma once
#include "transformation.h"

class identity final : public transformation
{
public:
    explicit identity(): transformation(matrix4::identity())
    {
    }
};
