/*
  ==============================================================================

    SourceCode.h
    Created: 20 Mar 2024 2:38:09pm
    Author:  cjia

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class xxxSlider : public juce::Slider
{
public:
    xxxSlider() {}

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

    double getValue() 
    {
        return 0;
    }




private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DecibelSlider)
};
