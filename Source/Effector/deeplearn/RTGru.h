/*
  ==============================================================================

    RTGru.h
    Created: 13 Feb 2025 5:04:36pm
    Author:  maple

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <RTNeural/RTNeural.h>
#include <melatonin_perfetto/melatonin_perfetto.h>

#include "../ProcessorBase.h"

class RTGruProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    RTGruProcessor(juce::AudioProcessorValueTreeState* apvts);
    ~RTGruProcessor()
    {
      Apvts->addParameterListener("ModIndexComboBox", this);
      Apvts->addParameterListener("cORcPPComboBox", this);

    }

    void parameterChanged(const juce::String& parameterID, float newValue)
    {
      if (parameterID.equalsIgnoreCase("ModIndexComboBox"))
      {
          // juce::Logger::outputDebugString("ModIndexComboBox");
          juce::Logger::outputDebugString("ModIndex =" + juce::String(newValue));
          SetMod((int)newValue);
      }
      if (parameterID.equalsIgnoreCase("cORcPPComboBox"))
      {
          // juce::Logger::outputDebugString("ModIndexComboBox");
          juce::Logger::outputDebugString("cORcPPindex =" + juce::String(newValue));
          cORcPPindex = (int)newValue;
      }
    }
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {

    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&); 


    void reset() override
    {

    }

    void SetMod(int);
    const juce::String getName() const override { return "RTGru"; }

private:
    RTNeural::ModelT<float, 1, 1,
        RTNeural::GRULayerT<float, 1, 9>,
        RTNeural::DenseT<float, 9, 1>> model;

    unsigned int modelIndex = 0;
    unsigned int cORcPPindex = 0;
    juce::AudioProcessorValueTreeState* Apvts;

    #if PERFETTO
    std::unique_ptr<perfetto::TracingSession> tracingSession;
    #endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RTGruProcessor)
};