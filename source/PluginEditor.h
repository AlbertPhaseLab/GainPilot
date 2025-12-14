#pragma once

#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "juce_gui_basics/juce_gui_basics.h"

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
    juce::Slider wetDrySlider;

    juce::Label gainLabel;
    juce::Label toneLabel;
    juce::Label wetDryLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        gainAttachment, toneAttachment, wetDryAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
