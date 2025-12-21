#pragma once

#include <juce_core/juce_core.h>

class ToneFilter
{
public:
    void prepare(double sampleRate)
    {
        fs = sampleRate;
        reset();
    }

    void reset()
    {
        z1 = 0.0f;
    }

    void setCutoff(float cutoffHz)
    {
        cutoff = cutoffHz;
        updateCoefficients();
    }

    inline float processSample(float x)
    {
        // One-pole low-pass: y[n] = a*x[n] + (1-a)*y[n-1]
        z1 = a * x + (1.0f - a) * z1;
        return z1;
    }

private:
    void updateCoefficients()
    {
        const float omega = 2.0f * juce::MathConstants<float>::twoPi * cutoff / float(fs);
        a = omega / (omega + 1.0f);
    }

    double fs = 44100.0;
    float cutoff = 1000.0f;
    float a = 0.1f;
    float z1 = 0.0f;
};
