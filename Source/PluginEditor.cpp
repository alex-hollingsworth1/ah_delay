/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AH_DELAYAudioProcessorEditor::AH_DELAYAudioProcessorEditor (AH_DELAYAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(gainKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(delayTimeKnob);
    setSize (500, 330);
}

AH_DELAYAudioProcessorEditor::~AH_DELAYAudioProcessorEditor()
{
}

//==============================================================================
void AH_DELAYAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void AH_DELAYAudioProcessorEditor::resized()
{
    delayTimeKnob.setTopLeftPosition(20, 10);
    mixKnob.setTopLeftPosition(delayTimeKnob.getRight() + 20, 10);
    gainKnob.setTopLeftPosition(mixKnob.getRight() + 20, 10);
}
