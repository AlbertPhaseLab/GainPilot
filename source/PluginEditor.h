#pragma once

#include "PluginProcessor.h"
#include "LookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AudioPluginAudioProcessor& processorRef;
    GainPilotLookAndFeel lookAndFeel;

    juce::Slider gainSlider;
    juce::Slider toneSlider;

    juce::Label gainLabel;
    juce::Label toneLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        gainAttachment, toneAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
