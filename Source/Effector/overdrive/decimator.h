//
// Created by maple on 25-4-18.
//

#ifndef DECIMATOR_H
#define DECIMATOR_H

#include <JuceHeader.h>
#include "../ProcessorBase.h"
#include "FIRProcessor.h"

class Decimator : public ProcessorBase
{
public:
    Decimator(){}
    ~Decimator(){}



    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        FIR_Processor.prepareToPlay(sampleRate, samplesPerBlock);
    }

    void decimateBuffer(juce::AudioBuffer<float>& buffer, int decimationFactor)
    {
        if (decimationFactor <= 1)
            return; // 不抽值

        const int numChannels = buffer.getNumChannels();
        const int originalNumSamples = buffer.getNumSamples();
        const int newNumSamples = originalNumSamples / decimationFactor;

        juce::AudioBuffer<float> newBuffer(numChannels, newNumSamples);

        for (int channel = 0; channel < numChannels; ++channel)
        {
            const float* src = buffer.getReadPointer(channel);
            float* dest = newBuffer.getWritePointer(channel);

            for (int i = 0; i < newNumSamples; ++i)
            {
                dest[i] = src[i * decimationFactor];
            }
        }

        // 拷贝回原 buffer
        buffer.setSize(numChannels, newNumSamples);
        for (int channel = 0; channel < numChannels; ++channel)
        {
            buffer.copyFrom(channel, 0, newBuffer, channel, 0, newNumSamples);
        }
    }



    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override
    {
        FIR_Processor.processBlock(buffer, midi);
        decimateBuffer(buffer, 2);
    }
    // 更新滤波器系数
    void updateCoefficients(const std::vector<float>& newCoeffs)
    {
        FIR_Processor.updateCoefficients(newCoeffs);
    }
private:


    FIRProcessor FIR_Processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Decimator)
};

#endif //DECIMATOR_H
