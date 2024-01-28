/*
  ==============================================================================

    SynthVoice.cpp
    Created: 1 Jan 2024 8:40:54pm
    Author:  cjia

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
    //如果可以运行，返回true
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    keylevel = velocity; //获取力度信息
    //juce::Logger::outputDebugString("frequency=" + juce::String(frequency));
    myEnv1.trigger = 1;
    //juce::Logger::outputDebugString("startNote=");
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    myEnv1.trigger = 0;
    //juce::Logger::outputDebugString("stopNote=");
}
void SynthVoice::getEnvAttack(float Attack, float Decay, float Sustain, float Release, int index)
{
    //juce::Logger::outputDebugString("index="+ juce::String(index)+"  frequency=" + juce::String(Attack));
    myEnv1.setAttack(Attack); 
    myEnv1.setDecay(Decay);
    myEnv1.setSustain(Sustain);
    myEnv1.setRelease(Release);
    //juce::Logger::outputDebugString("index="+ juce::String(index)+"  Attack=" + juce::String(Attack));
    //juce::Logger::outputDebugString("index=" + juce::String(index) + "  Decay=" + juce::String(Decay));
    //juce::Logger::outputDebugString("index=" + juce::String(index) + "  Sustain=" + juce::String(Sustain));
    //juce::Logger::outputDebugString("index=" + juce::String(index) + "  Release=" + juce::String(Release));

}
void SynthVoice::getOsc(bool SineButton, bool SawButton, bool PhasorButton, bool NoiseButton, int index)
{
        Sinebool = SineButton;
        Sawbool = SawButton;
        Squarebool = PhasorButton;
        Noisebool = NoiseButton;
}
double SynthVoice::OscOutput()
{
        double OutPut=0;
        if (Sinebool == true)
        {
            OutPut += myOsc1.sinewave(frequency);
        }
        if (Sawbool == true)
        {
            OutPut += myOsc1.saw(frequency);
        }
        if (Squarebool == true)
        {
            OutPut += myOsc1.square(frequency);
        }
        if (Noisebool == true)
        {
            OutPut += myOsc1.noise();
        }
        return OutPut;
}
void SynthVoice::getLevel(float Level, int index)
{
    userlevel = juce::Decibels::decibelsToGain(Level);
}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
    //myEnv1.setAttack(500);   //A
    //myEnv1.setDecay(500);    //D
    //myEnv1.setSustain(0.8);  //S
    //myEnv1.setRelease(100);  //R
    //juce::Logger::outputDebugString("renderNextBlock");
    for (auto sample = 0; sample < numSamples; ++sample)//将maxiOsc对象生成的音频数据逐样本地填充各个通道的到缓冲区
    {
        auto myWave = OscOutput() * keylevel * userlevel;
        auto soundOfEnv = myEnv1.adsr(myWave, myEnv1.trigger);//对振荡器的音频信号应用包络

        for (auto channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample, soundOfEnv);
        }
        ++startSample;
    }
}


//void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
//{
//    myEnv1.setAttack(500);   //A
//    myEnv1.setDecay(100);    //D
//    myEnv1.setSustain(0.8);  //S
//    myEnv1.setRelease(500);  //R
//    for (auto sample = 0; sample < numSamples; ++sample)
//    {
//        auto myWave = myOsc1.sinewave(frequency) * level;
//        auto soundOfEnv = myEnv1.adsr(myWave, myEnv1.trigger);//对振荡器的音频信号应用包络
//        for (auto channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
//        {
//            outputBuffer.addSample(channel, startSample, soundOfEnv);//包络处理后的音频信号传入缓存中
//        }
//        ++startSample;
//    }
//}
