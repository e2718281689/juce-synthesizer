/*
  ==============================================================================

    ProcessorMidi.h
    Created: 31 Dec 2023 10:21:01pm
    Author:  cjia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Synth/SynthSound.h"
#include "../Synth/SynthVoice.h"
class ProcessorMidi
{
public:
	//ProcessorMidi();
	//~ProcessorMidi();
    void ProcessorMidi::process(juce::MidiBuffer& midiMessages, juce::Synthesiser& v_synth);
private:
    juce::HashMap<juce::String, void *> key_Voice;
    juce::HashMap<juce::String, void *> key_Sound;
    int key_Voice_index;
    int key_Sound_index;
};

