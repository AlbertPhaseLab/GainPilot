#include "LookAndFeel.h"

GainPilotLookAndFeel::GainPilotLookAndFeel()
{
    setColour(juce::Slider::thumbColourId, juce::Colours::grey);
    setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightgrey);
}

GainPilotLookAndFeel::~GainPilotLookAndFeel() {}

void GainPilotLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPosProportional, float rotaryStartAngle,
                                         float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(10.f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(juce::Colours::darkgrey);
    g.fillEllipse(bounds);

    juce::Path p;
    auto pointerLength = radius * 0.6f;
    auto pointerThickness = 4.0f;
    p.addRectangle(-pointerThickness/2, -pointerLength, pointerThickness, pointerLength);
    g.setColour(juce::Colours::grey);
    g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
}
