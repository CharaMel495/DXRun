#pragma once
#include "CaramelEngine.h"
#include <random>

float getRandomFloat(float min, float max)
{
    static std::mt19937 engine{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine);
}
