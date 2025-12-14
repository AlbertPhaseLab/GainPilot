#pragma once
#include "juce_gui_basics/juce_gui_basics.h"

class GainPilotLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GainPilotLookAndFeel()
    {
        setColour(juce::Slider::rotarySliderFillColourId, accentColour);
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    void drawRotarySlider(juce::Graphics& g,
                          int x, int y, int width, int height,
                          float sliderPosProportional,
                          float rotaryStartAngle,
                          float rotaryEndAngle,
                          juce::Slider&) override
    {
        auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(10);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
        auto centre = bounds.getCentre();
        auto angle = rotaryStartAngle + sliderPosProportional
                     * (rotaryEndAngle - rotaryStartAngle);

        // Background circle
        g.setColour(backgroundKnob);
        g.fillEllipse(bounds);

        // Arc
        juce::Path arc;
        arc.addCentredArc(centre.x, centre.y, radius, radius,
                          0.0f, rotaryStartAngle, angle, true);

        g.setColour(accentColour);
        g.strokePath(arc, juce::PathStrokeType(4.0f));

        // Pointer
        juce::Path pointer;
        pointer.addRectangle(-2.0f, -radius, 4.0f, radius * 0.6f);

        g.setColour(juce::Colours::white);
        g.fillPath(pointer,
                   juce::AffineTransform::rotation(angle).translated(centre));
    }

private:
    juce::Colour backgroundKnob = juce::Colour(0xff2b2b2b);
    juce::Colour accentColour   = juce::Colour(0xff3daee9);
};
