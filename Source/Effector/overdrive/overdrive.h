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
        DigitDistortion1_Unit.enable =1;
        DigitDistortion1_Unit.ct = &DigitDistortion1Con_text;
        DigitDistortion1_Unit.mix =1;
        DigitDistortion1_Unit.low =1;
        DigitDistortion1_Unit.high =1;
        DigitDistortion1_Unit.gain = 3;
        DigitDistortion1_Unit.mode = 1;
        DigitDistortion1_Unit.sample_rate = sampleRate;
        AudioEffectDigitDistortion1Init(&DigitDistortion1_Unit,sampleRate);
        AudioEffectDigitDistortion1Configure(&DigitDistortion1_Unit,1,1,1,1,1);
    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        float *buff_R = buffer.getWritePointer(0);
        float *buff_L = buffer.getWritePointer(1);
        uint32_t num = buffer.getNumSamples();

        AudioEffectDigitDistortion1Apply(&DigitDistortion1_Unit,buff_R,buff_R,num);
        // AudioEffectDigitDistortion1Apply(&DigitDistortion1_Unit,buff_L,buff_L,num);
        memcpy(buff_L, buff_R, num * sizeof(float));



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