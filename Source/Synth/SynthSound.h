/*
  ==============================================================================

    SynthSound.h
    Created: 1 Jan 2024 1:55:08pm
    Author:  cjia

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }

};