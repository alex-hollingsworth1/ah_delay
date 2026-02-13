#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ProtectYourEars.h"


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
    return 1;
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
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    
    delayLine.prepare(spec);
    
    double numSamples = Parameters::maxDelayTime / 1000.0 * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLine.setMaximumDelayInSamples(maxDelayInSamples);
    delayLine.reset();
    
    feedbackL = 0.0f;
    feedbackR = 0.0f;
    
}

void AH_DELAYAudioProcessor::releaseResources()
{
}

bool AH_DELAYAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();
    
    if (mainIn == mono && mainOut == mono) { return true; }
    if (mainIn == mono && mainOut == stereo) { return true; }
    if (mainIn == stereo && mainOut == stereo) { return true; }
    
    return false;
}

void AH_DELAYAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.update();
    
    float sampleRate = float(getSampleRate());
    
    auto mainInput = getBusBuffer(buffer, true, 0);
    auto mainInputChannels = mainInput.getNumChannels();
    auto isMainInputStereo = mainInputChannels > 1;
    const float* inputDataL = mainInput.getReadPointer(0);
    const float* inputDataR = mainInput.getReadPointer(isMainInputStereo ? 1 : 0);
    
    auto mainOutput = getBusBuffer(buffer, false, 0);
    auto mainOutputChannels = mainOutput.getNumChannels();
    auto isMainOutputStereo = mainOutputChannels > 1;
    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(isMainOutputStereo ? 1 : 0);
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        params.smoothen();
        float delayInSamples = (params.delayTime / 1000.0f) * sampleRate;
        delayLine.setDelay(delayInSamples);
        
        float dryL = inputDataL[sample];
        float dryR = inputDataR[sample];
        
        // Convert stereo to mono
        float mono = (dryL + dryR) * 0.5f;
        
        delayLine.pushSample(0, mono*params.panL + feedbackR);
        delayLine.pushSample(1, mono*params.panR + feedbackL);
        
        float wetL = delayLine.popSample(0);
        float wetR = delayLine.popSample(1);
        
        feedbackL = wetL * params.feedback;
        feedbackR = wetR * params.feedback;
        
        float mixL = dryL + wetL * params.mix;
        float mixR = dryR + wetR * params.mix;
        
        outputDataL[sample] = mixL * params.gain;
        outputDataR[sample] = mixR * params.gain;
        
        #if JUCE_DEBUG
        protectYourEars(buffer);
        #endif
    }
}

//==============================================================================
bool AH_DELAYAudioProcessor::hasEditor() const
{
    return true;
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


