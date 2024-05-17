#include "matrix4.h"

matrix4 matrix4::operator*(const matrix4& matrix) const
{
    matrix4 result{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                result.elements_[i][j] += elements_[i][k] * matrix.elements_[k][j];
    return result;
}

matrix4 matrix4::identity()
{
    matrix4 result{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (i == j)
                result.elements_[i][j] = 1.0f;
            else
                result.elements_[i][j] = 0.0f;
    return result;
}
