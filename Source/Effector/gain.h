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

class GainProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    GainProcessor()
    {

    }
    ~GainProcessor()
    {
        Apvts->removeParameterListener("levelSlider", this);
    }
    void init(juce::AudioProcessorValueTreeState *apvts) override
    {
        Apvts = apvts;
        Apvts->addParameterListener("levelSlider", this);
        Gain_Level = Apvts->getParameterAsValue("levelSlider").getValue();

    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {
        if (parameterID.equalsIgnoreCase("levelSlider"))
        {
            Gain_Level = juce::Decibels::decibelsToGain(newValue);
            juce::Logger::outputDebugString("Gain_Level = " +  juce::String(Gain_Level));
        }
    }
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {

    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            for (int j = 0; j < getTotalNumOutputChannels(); j++)
            {
                buffer.setSample(j, i, buffer.getSample(j, i) * Gain_Level);
            }
        }
    }

    void reset() override
    {
        
    }


    const juce::String getName() const override { return "Gain"; }

private:
    float Gain_Level = 0.0f;
    juce::AudioProcessorValueTreeState *Apvts;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainProcessor)
};