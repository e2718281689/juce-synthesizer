/*
  ==============================================================================

    filter.h
    Created: 9 May 2024 4:09:55pm
    Author:  cjia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ProcessorBase.h"

class testProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    testProcessor()
    {

    }
    ~testProcessor()
    {


    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {
    }
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
    }
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(testProcessor)
};