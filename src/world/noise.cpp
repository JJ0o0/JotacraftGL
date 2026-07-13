#include <world/noise.hpp>

Noise::Noise() {
    m_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    m_noise.SetFrequency(0.01f);
}

float Noise::Sample(float x, float z) const { return m_noise.GetNoise(x, z); }