#pragma once
#include <initializer_list>

class matrix4
{
    float elements_[4][4];

public:
    matrix4(const std::initializer_list<float> elements)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                elements_[i][j] = *(elements.begin() + i * 4 + j);
    }

    matrix4 operator*(const matrix4& matrix) const;

    static matrix4 identity();
};
