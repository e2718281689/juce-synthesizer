/*
  ==============================================================================

    SynthVoice.h
    Created: 1 Jan 2024 1:55:17pm
    Author:  cjia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "../Maximilian/src/maximilian.h"
class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;
    void getEnvAttack(float Attack, float Decay, float Sustain, float Release, int index);
    void getOsc(bool SineButton, bool SawButton, bool PhasorButton, bool NoiseButton, int index);
    void getLevel(float Level,int index);
    double OscOutput();
private:
    double frequency;
    double keylevel;
    double userlevel=0.5f;
    maxiOsc myOsc1;
    maxiEnv myEnv1;
    bool Sinebool=false, Sawbool = false, Squarebool = false, Noisebool = false;
};