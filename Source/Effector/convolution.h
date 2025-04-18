/*
  ==============================================================================

    convolution.h
    Created: 25 May 2024 1:16:29pm
    Author:  cjia

  ==============================================================================
*/



#pragma once

#include <JuceHeader.h>
#include "ProcessorBase.h"

class ConvolutionProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    ConvolutionProcessor(juce::AudioProcessorValueTreeState* apvts)
    {
        Apvts = apvts;
        Apvts->addParameterListener("testslider", this);
        Apvts->addParameterListener("GainSlider", this);
        juce::Logger::outputDebugString("FilterProcessor");

        testslider_date = Apvts->getParameterAsValue("testslider").getValue();

        channels = juce::jmax(getTotalNumInputChannels(), getTotalNumOutputChannels());

    }
    ~ConvolutionProcessor()
    {
        Apvts->removeParameterListener("testslider", this);
        Apvts->removeParameterListener("GainSlider", this);

    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {
        if (parameterID.equalsIgnoreCase("testslider"))
        {
        }
        if (parameterID.equalsIgnoreCase("GainSlider"))
        {
        }
    }
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {


    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        //filter.process(context);
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            for (int j = 0; j < getTotalNumOutputChannels(); j++)
            {
                buffer.setSample(j, i, sin(buffer.getSample(j, i) * testslider_date));
            }
        }
    }

    void reset() override
    {

    }

    juce::AudioProcessorValueTreeState* Apvts;
    const juce::String getName() const override { return "convolution"; }

private:
    float testslider_date = 0;
    int SampleRate = 0;
    int channels = 0;

};