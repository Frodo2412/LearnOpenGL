#pragma once

class vertex
{
    float position_[3], color_[3], texture_[2];

public:
    vertex(const float x, const float y, const float z, const float r, const float g, const float b,
           const float u, const float v): position_{x, y, z}, color_{r, g, b}, texture_{u, v}
    {
    }

};
