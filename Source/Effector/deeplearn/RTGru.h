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

#include "../ProcessorBase.h"
#include "../cRT/deeplearn.h"

class RTGruProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
  RTGruProcessor();
  ~RTGruProcessor();
  void init(juce::AudioProcessorValueTreeState *apvts);
  void parameterChanged(const juce::String &parameterID, float newValue);
  // void prepareToPlay(double sampleRate, int samplesPerBlock); 
  void processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &);
  // void reset(); 

  void SetMod(int mod); 
  const juce::String getName() const override { return "RTGru"; }

private:
  RTNeural::ModelT<float, 1, 1,
                   RTNeural::GRULayerT<float, 1, 9>,
                   RTNeural::DenseT<float, 9, 1>>
      model;

  unsigned int modelIndex = 0;
  unsigned int cORcPPindex = 0;
  juce::AudioProcessorValueTreeState *Apvts;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RTGruProcessor)
};