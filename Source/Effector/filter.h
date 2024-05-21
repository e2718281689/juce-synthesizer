/*
  ==============================================================================

    filter.h
    Created: 9 May 2024 4:09:55pm
    Author:  cjia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class ProcessorBase : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ProcessorBase()
        : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
            .withOutput("Output", juce::AudioChannelSet::stereo()))
    {}

    //==============================================================================
    void prepareToPlay(double, int) override {}
    void releaseResources() override {}
    void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) override {}

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }

    //==============================================================================
    const juce::String getName() const override { return {}; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return 0; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    //==============================================================================
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}
    //==============================================================================
    void parameterChanged(const juce::String& parameterID, float newValue) override {};
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorBase)
};

class FilterProcessor : public ProcessorBase
{
public:
    FilterProcessor(juce::AudioProcessorValueTreeState* apvts)
    {
        Apvts = apvts;
        Apvts->addParameterListener("testslider", this);
        Apvts->addParameterListener("xxxtestslider", this);
    }
    ~FilterProcessor()
    {
        Apvts->removeParameterListener("testslider", this);
        Apvts->removeParameterListener("xxxtestslider", this);

    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {

        if (parameterID.equalsIgnoreCase("testslider"))
        {
            *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), juce::jmap<float>(newValue, -72.0, 10, 10, 1000));
        }
        if (parameterID.equalsIgnoreCase("xxxtestslider"))
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
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;
};