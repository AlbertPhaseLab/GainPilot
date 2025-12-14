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

    // Attach sliders to parameters
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "gain", gainSlider);
    toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "tone", toneSlider);

    // Labels configuration
    auto configureLabel = [this](juce::Label& label, const juce::String& text)
    {
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
    };

    configureLabel(gainLabel, "Gain (dB)");
    configureLabel(toneLabel, "Tone");

    // Editor size
    setSize(400, 200);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
    toneSlider.setLookAndFeel(nullptr);
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
    int sliderSize = 100;
    int sliderSpacing = 50; // space between sliders
    int labelHeight = 20;

    int startX = (getWidth() - 2 * sliderSize - sliderSpacing) / 2;
    int ySlider = 60;

    // Gain positioning
    gainSlider.setBounds(startX, ySlider, sliderSize, sliderSize);
    gainLabel.setBounds(gainSlider.getX(), gainSlider.getBottom() + 5, sliderSize, labelHeight);

    // Tone positioning
    toneSlider.setBounds(startX + sliderSize + sliderSpacing, ySlider, sliderSize, sliderSize);
    toneLabel.setBounds(toneSlider.getX(), toneSlider.getBottom() + 5, sliderSize, labelHeight);
}
