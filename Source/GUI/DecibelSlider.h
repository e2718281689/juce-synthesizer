/*
  ==============================================================================

    DecibelSlider.h
    Created: 25 Oct 2023 8:17:48pm
    Author:  cjia

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DecibelSlider : public juce::Slider
{
public:
    DecibelSlider() {}

    double getValueFromText(const juce::String& text) override
    {
        auto minusInfinitydB = -100.0;

        auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();    // [1]

        return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB
            : decibelText.getDoubleValue();  // [2]
    }

    juce::String getTextFromValue(double value) override
    {
        return juce::Decibels::toString(value);
    }
    


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DecibelSlider)
};
