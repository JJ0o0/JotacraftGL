#pragma once

#include <noise/FastNoiseLite.h>

class Noise {
    public:
        Noise();

        float Sample(float x, float z) const;
    private:
        FastNoiseLite m_noise;
};