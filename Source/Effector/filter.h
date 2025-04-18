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

class FilterProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    FilterProcessor(juce::AudioProcessorValueTreeState* apvts)
    {
        Apvts = apvts;
        Apvts->addParameterListener("testslider", this);
        Apvts->addParameterListener("GainSlider", this);
        juce::Logger::outputDebugString("FilterProcessor");

        testslider_date= Apvts->getParameterAsValue("testslider").getValue();
    }
    ~FilterProcessor()
    {
        Apvts->removeParameterListener("testslider", this);
        Apvts->removeParameterListener("GainSlider", this);

    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {

        if (parameterID.equalsIgnoreCase("testslider"))
        {
            testslider_date = newValue;
            *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), juce::jmap<float>(testslider_date, -72.0, 10, 10, 1000));
        }
        if (parameterID.equalsIgnoreCase("GainSlider"))
        {
        }
    }
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 1000.0f);
        juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
        filter.prepare(spec);
    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        filter.process(context);
    }

    void reset() override
    {
        filter.reset();
    }

    juce::AudioProcessorValueTreeState* Apvts;
    const juce::String getName() const override { return "Filter"; }

private:
    float testslider_date = 0;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterProcessor)
};