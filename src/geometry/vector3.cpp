#include "vector3.h"

float vector3::get_x() const
{
    return coordinates_[0];
}

float vector3::get_y() const
{
    return coordinates_[1];
}

float vector3::get_z() const
{
    return coordinates_[2];
}

vector3 vector3::operator+(const vector3& other) const
{
    return {
        coordinates_[0] + other.coordinates_[0],
        coordinates_[1] + other.coordinates_[1],
        coordinates_[2] + other.coordinates_[2]
    };
}

vector3 vector3::operator-(const vector3& other) const
{
    return {
        coordinates_[0] - other.coordinates_[0],
        coordinates_[1] - other.coordinates_[1],
        coordinates_[2] - other.coordinates_[2]
    };
}

vector3 vector3::operator*(const float scalar) const
{
    return {
        coordinates_[0] * scalar,
        coordinates_[1] * scalar,
        coordinates_[2] * scalar
    };
}

vector3 vector3::operator/(const float scalar) const
{
    return {
        coordinates_[0] / scalar,
        coordinates_[1] / scalar,
        coordinates_[2] / scalar
    };
}

vector3 vector3::normalize() const
{
    return *this / norm_;
}
