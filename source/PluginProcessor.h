#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "ToneFilter.h"

//==============================================================================

struct PluginParameters
{
    std::atomic<float>* gain = nullptr;
    std::atomic<float>* tone = nullptr;
    std::atomic<float>* wetDry = nullptr;

    void init(juce::AudioProcessorValueTreeState& apvts)
    {
        gain = apvts.getRawParameterValue("gain");
        tone = apvts.getRawParameterValue("tone");
        wetDry = apvts.getRawParameterValue("wetDry");
    }

    // Avoid accidental copies
    PluginParameters(const PluginParameters&) = delete;
    PluginParameters& operator=(const PluginParameters&) = delete;

    // Can be moved
    PluginParameters(PluginParameters&&) = default;
    PluginParameters& operator=(PluginParameters&&) = default;

    PluginParameters() = default;
};

class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //== Parameters ==
    juce::AudioProcessorValueTreeState apvts;

    PluginParameters params;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)

    ToneFilter toneFilterR;
    ToneFilter toneFilterL;
};
