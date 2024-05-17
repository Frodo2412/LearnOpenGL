#include "transformation.h"

matrix4 transformation::get_matrix() const
{
    return matrix_;
}

transformation transformation::operator>>(const transformation& other) const
{
    return transformation(matrix_ * other.matrix_);
}
