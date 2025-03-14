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
#include "../wavenet/wavenet_model.hpp"

class WaveNetProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
  public:

    WaveNetProcessor();
    ~WaveNetProcessor();
    void init(juce::AudioProcessorValueTreeState *apvts) override ;
    void parameterChanged(const juce::String &parameterID, float newValue);
    // void prepareToPlay(double sampleRate, int samplesPerBlock); 
    void processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &);
    // void reset(); 

    void SetMod(int mod); 
    const juce::String getName() const override { return "WaveNet"; }

  private:

    using Dilations = wavenet::Dilations<1, 2, 4, 8, 16, 32, 64>;
    using Dilations2 = wavenet::Dilations<128, 256, 512, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512>;
    wavenet::Wavenet_Model<float,
                          1,
                          wavenet::Layer_Array<float, 1, 1, 2, 2, 3, Dilations, false, NAMMathsProvider>,
                          wavenet::Layer_Array<float, 2, 1, 1, 2, 3, Dilations2, true, NAMMathsProvider>>
    rtneural_wavenet; 

    juce::AudioProcessorValueTreeState *Apvts;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveNetProcessor)
};