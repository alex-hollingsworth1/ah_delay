/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AH_DELAYAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AH_DELAYAudioProcessorEditor)
};
