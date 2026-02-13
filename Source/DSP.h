/*
  ==============================================================================

    DSP.h
    Created: 12 Feb 2026 11:34:08pm
    Author:  Alex Hollingsworth

  ==============================================================================
*/

#pragma once

#include <cmath>

inline void panningEqualPower(float panning, float& left, float& right)
{
    float x = 0.7853981633974483f * (panning + 1.0f);
    left = std::cos(x);
    right = std::sin(x);
}


