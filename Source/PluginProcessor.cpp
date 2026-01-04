/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AH_DELAYAudioProcessor::AH_DELAYAudioProcessor():
    AudioProcessor(
                   BusesProperties()
                   .withInput("Input", juce::AudioChannelSet::stereo(), true)
                   .withOutput("Output", juce::AudioChannelSet::stereo(), true)
   ),
    params(apvts)
{
    // do nothing
}

AH_DELAYAudioProcessor::~AH_DELAYAudioProcessor()
{
}

//==============================================================================
const juce::String AH_DELAYAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AH_DELAYAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AH_DELAYAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AH_DELAYAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AH_DELAYAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AH_DELAYAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AH_DELAYAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AH_DELAYAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AH_DELAYAudioProcessor::getProgramName (int index)
{
    return {};
}

void AH_DELAYAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AH_DELAYAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
}

void AH_DELAYAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AH_DELAYAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void AH_DELAYAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.update();
    
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        params.smoothen();
        
        channelDataL[sample] *= params.gain;
        channelDataR[sample] *= params.gain;
    }
}

//==============================================================================
bool AH_DELAYAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AH_DELAYAudioProcessor::createEditor()
{
    return new AH_DELAYAudioProcessorEditor (*this);
}

//==============================================================================
void AH_DELAYAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void AH_DELAYAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AH_DELAYAudioProcessor();
}


