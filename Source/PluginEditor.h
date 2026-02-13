/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class AH_DELAYAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AH_DELAYAudioProcessorEditor (AH_DELAYAudioProcessor&);
    ~AH_DELAYAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AH_DELAYAudioProcessor& audioProcessor;
    
    RotaryKnob gainKnob{"Gain", audioProcessor.apvts, gainParamID, true};
    RotaryKnob mixKnob{"Mix", audioProcessor.apvts, mixParamID};
    RotaryKnob delayTimeKnob{"Time", audioProcessor.apvts, delayTimeParamID};
    RotaryKnob feedbackKnob{"Feedback", audioProcessor.apvts, feedbackParamID, true};
    RotaryKnob stereoKnob { "Stereo", audioProcessor.apvts, stereoParamID, true };
    MainLookAndFeel mainLF;
    
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AH_DELAYAudioProcessorEditor)
};
