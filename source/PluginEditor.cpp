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

    gainSlider.setLookAndFeel(&lookAndFeel);

    setSize (300, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));

    // Header
    g.setColour(juce::Colour(0xff3daee9));
    g.setFont(juce::Font(22.0f, juce::Font::bold));
    g.drawText("GainPilot", 0, 10, getWidth(), 30,
               juce::Justification::centred);

    // Subtitle
    g.setColour(juce::Colours::grey);
    g.setFont(12.0f);
    g.drawText("Precision Gain Control",
               0, 40, getWidth(), 20,
               juce::Justification::centred);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(30);
    area.removeFromTop(60);
    gainSlider.setBounds(area);
    gainLabel.setBounds(0, getHeight() - 40, getWidth(), 30);
}
