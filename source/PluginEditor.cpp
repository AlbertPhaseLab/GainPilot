#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    //juce::ignoreUnused (processorRef);
    // Slider setup
    gainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    gainSlider.setRange(0.0, 24.0, 0.01);

    gainAttachment = std::make_unique<SliderAttachment>(
        processorRef.parameters, "gain", gainSlider);

    addAndMakeVisible(gainSlider);

    // Label setup
    gainLabel.setText("Gain (dB)", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    setSize (300, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("GainPilot", getLocalBounds().removeFromTop(40),
                     juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto area = getLocalBounds();

    auto title = area.removeFromTop(50);

    gainSlider.setBounds(area.reduced(40));
    gainLabel.setBounds(area.removeFromBottom(40));
}
