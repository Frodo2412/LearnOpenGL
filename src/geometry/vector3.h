#pragma once
#include <valarray>

class vector3 final
{
    float coordinates_[3], norm_;

public:
    vector3(const float x, const float y, const float z): coordinates_{x, y, z}, norm_(std::sqrt(x * x + y * y + z * z))
    {
    }

    // Getters
    float get_x() const;
    float get_y() const;
    float get_z() const;

    // Vector operators
    vector3 operator+(const vector3& other) const;
    vector3 operator-(const vector3& other) const;

    // Scalar operators
    vector3 operator*(float scalar) const;
    vector3 operator/(float scalar) const;

    // Other methods
    vector3 normalize() const;
};
