/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MIDI/ProcessorMidi.h"
#include "Maximilian/src/maximilian.h"

#include "Synth/SynthSound.h"
#include "Synth/SynthVoice.h"
#include "GUI/AudioFIFO.h"
#include "Effector/filter.h"
#include "Effector/ProcessorGroup.h"
//==============================================================================
/**
*/
class ScscAudioProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif


{
public:
    //==============================================================================
    ScscAudioProcessor();
    ~ScscAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;
    void reset() override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void ScscAudioProcessor::CabSimulator();

    SingleChannelSampleFifo<juce::AudioBuffer<float>>& getSingleChannelSampleFifo() { return singleChannelSampleFifo; }
    juce::AudioProcessorValueTreeState apvts;

    void parameterChanged(const juce::String& parameterID, float newValue);

    float EnvAttackTime, EnvDecayTime, EnvReleaseTime, EnvSustainTime;

    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = juce::AudioProcessorGraph::Node;

private:
    //==============================================================================
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.01;
    double xxxcurrentSampleRate = 0.0, xxxcurrentAngle = 0.0, xxxangleDelta = 0.01;
    juce::Random random;
    ProcessorMidi Pmidi;
    SynthVoice* myVoice;
    juce::Synthesiser synth;
    juce::AudioProcessorValueTreeState::ParameterLayout CreateParameters();
    SingleChannelSampleFifo<juce::AudioBuffer<float>> singleChannelSampleFifo{ 1 };

    //std::unique_ptr<juce::AudioProcessorGraph> mainProcessor{ new juce::AudioProcessorGraph() };

    std::unique_ptr<ProcessorGroup> mainProcessor{ new ProcessorGroup() };
    enum
    {
        convolutionIndex 
    };

    Node::Ptr FilterNode;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScscAudioProcessor)
};
