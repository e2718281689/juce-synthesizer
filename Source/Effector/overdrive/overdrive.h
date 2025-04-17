/*
  ==============================================================================

    filter.h
    Created: 9 May 2024 4:09:55pm
    Author:  cjia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ProcessorBase.h"
#include "DigitDistortion1.h"

class overdrive : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    overdrive()
    {

//        DigitDistortion1_Unit.
    }
    ~overdrive()
    {
        Apvts->removeParameterListener("levelSlider", this);
    }
    void init(juce::AudioProcessorValueTreeState *apvts) override
    {
        Apvts = apvts;
        Apvts->addParameterListener("levelSlider", this);

    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {
        if (parameterID.equalsIgnoreCase("levelSlider"))
        {

        }
    }
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        // DigitDistortion1_Unit.enable =1;
        // DigitDistortion1_Unit.ct = &DigitDistortion1Con_text;
        // DigitDistortion1_Unit.mix =0;
        // DigitDistortion1_Unit.low =0;
        // DigitDistortion1_Unit.high =0;
        // DigitDistortion1_Unit.gain = 0;
        // DigitDistortion1_Unit.mode = 0;
        // DigitDistortion1_Unit.sample_rate = sampleRate;
        // AudioEffectDigitDistortion1Init(&DigitDistortion1_Unit,sampleRate);
    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {

    }

    void reset() override
    {
        
    }


    const juce::String getName() const override { return "overdrive"; }

private:

    DigitDistortion1Unit DigitDistortion1_Unit;
    DigitDistortion1Context DigitDistortion1Con_text;


    juce::AudioProcessorValueTreeState *Apvts;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(overdrive)
};