#pragma once
#include "../geometry/matrix4.h"

class transformation
{
    matrix4 matrix_;

protected:
    explicit transformation(const matrix4& matrix4): matrix_(matrix4)
    {
    }

public:
    matrix4 get_matrix() const;
    transformation operator>>(const transformation& other) const;
};
