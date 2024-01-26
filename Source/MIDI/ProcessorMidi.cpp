/*
  ==============================================================================

    ProcessorMidi.cpp
    Created: 31 Dec 2023 10:21:01pm
    Author:  cjia

  ==============================================================================
*/

#include "ProcessorMidi.h"
void ProcessorMidi::process(juce::MidiBuffer& midiMessages, juce::Synthesiser& v_synth)
{
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage currentMessage;
    int samplePos;

    while (it.getNextEvent(currentMessage, samplePos))
    {
        //juce::Logger::outputDebugString(currentMessage.getDescription());
        //currentMessage.isNoteOn();
        if (currentMessage.isNoteOn())
        {
            juce::Logger::outputDebugString("key_on" + juce::String(currentMessage.getNoteNumber()));
            key_Voice.set(juce::String(currentMessage.getNoteNumber()), v_synth.addVoice(new SynthVoice()));
            key_Sound.set(juce::String(currentMessage.getNoteNumber()), v_synth.addSound(new SynthSound()));

        }
        if (currentMessage.isNoteOff())
        {
            juce::Logger::outputDebugString("key_off" + juce::String(currentMessage.getNoteNumber()));
            for (int i = 0; i < v_synth.getNumVoices(); i++)
            {
                //juce::Logger::outputDebugString(v_synth.getSound(i));
            }

        }

    }
}