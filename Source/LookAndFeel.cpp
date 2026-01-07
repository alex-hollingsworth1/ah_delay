/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 7 Jan 2026 11:02:15am
    Author:  Alex Hollingsworth

  ==============================================================================
*/

#include "LookAndFeel.h"

RotaryKnobLookAndFeel::RotaryKnobLookAndFeel()
{
    setColour(juce::Label::textColourId, Colors::Knob::label);
    setColour(juce::Slider::textBoxTextColourId, Colors::Knob::label);
}
