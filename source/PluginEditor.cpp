#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Sliders conriguration
    auto configureSlider = [this](juce::Slider& slider)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setLookAndFeel(&lookAndFeel);
        addAndMakeVisible(slider);
    };

    configureSlider(gainSlider);
    configureSlider(toneSlider);
    configureSlider(wetDrySlider);

    // Attach sliders to parameters
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "gain", gainSlider);
    toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "tone", toneSlider);
    wetDryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "wetDry", wetDrySlider);

    // Labels configuration
    auto configureLabel = [this](juce::Label& label, const juce::String& text)
    {
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
    };

    configureLabel(gainLabel, "Gain (dB)");
    configureLabel(toneLabel, "Tone");
    configureLabel(wetDryLabel, "Wet/Dry");

    // Editor size
    setSize(500, 200);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
    toneSlider.setLookAndFeel(nullptr);
    wetDrySlider.setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));

    // Header
    g.setColour(juce::Colour(0xff3daee9));
    g.setFont(juce::Font(22.0f, juce::Font::bold));
    g.drawText("GainPilot", 0, 10, getWidth(), 30, juce::Justification::centred);

    // Subtitle
    g.setColour(juce::Colours::grey);
    g.setFont(12.0f);
    g.drawText("Precision Gain Control", 0, 40, getWidth(), 20, juce::Justification::centred);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::resized()
{
    int numSliders = 3; // Gain, Tone, Wet/Dry
    int sliderSize = 100; // Width and height of each slider
    int sliderSpacing = 50; // Space between sliders
    int labelHeight = 20; // Height of each label
    int ySlider = 60; // Y position for sliders

    // Total width calculation
    int totalWidth = numSliders * sliderSize + (numSliders - 1) * sliderSpacing;

    // Starting X position to center sliders
    int startX = (getWidth() - totalWidth) / 2;

    // Gain
    gainSlider.setBounds(startX, ySlider, sliderSize, sliderSize);
    gainLabel.setBounds(gainSlider.getX(), gainSlider.getBottom() + 5, sliderSize, labelHeight);

    // Tone
    toneSlider.setBounds(startX + sliderSize + sliderSpacing, ySlider, sliderSize, sliderSize);
    toneLabel.setBounds(toneSlider.getX(), toneSlider.getBottom() + 5, sliderSize, labelHeight);

    // Wet/Dry
    wetDrySlider.setBounds(startX + 2 * (sliderSize + sliderSpacing), ySlider, sliderSize, sliderSize);
    wetDryLabel.setBounds(wetDrySlider.getX(), wetDrySlider.getBottom() + 5, sliderSize, labelHeight);
}
