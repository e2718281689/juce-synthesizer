//
// Created by maple on 25-4-18.
//

#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H


#include <JuceHeader.h>
#include "../ProcessorBase.h"
#include "FIRProcessor.h"

class Interpolator : public ProcessorBase
{
public:
    Interpolator(){}
    ~Interpolator(){}



    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        FIR_Processor.prepareToPlay(sampleRate, samplesPerBlock);
    }

    void insertZeros(juce::AudioBuffer<float>& buffer, int zerosToInsert)
    {
        if (zerosToInsert <= 0)
            return; // 不插零直接返回

        const int numChannels = buffer.getNumChannels();
        const int originalNumSamples = buffer.getNumSamples();
        const int newNumSamples = originalNumSamples * (1 + zerosToInsert);

        // 创建一个更大的新 buffer 用于放置插零后的数据
        juce::AudioBuffer<float> newBuffer(numChannels, newNumSamples);
        newBuffer.clear(); // 默认清零

        for (int channel = 0; channel < numChannels; ++channel)
        {
            const float* src = buffer.getReadPointer(channel);
            float* dest = newBuffer.getWritePointer(channel);

            for (int i = 0; i < originalNumSamples; ++i)
            {
                dest[i * (1 + zerosToInsert)] = src[i];
                // 其余插零位置已被 clear()
            }
        }

        // 将新 buffer 拷贝回原始 buffer
        buffer.setSize(numChannels, newNumSamples);
        for (int channel = 0; channel < numChannels; ++channel)
        {
            buffer.copyFrom(channel, 0, newBuffer, channel, 0, newNumSamples);
        }
    }


    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override
    {

        // insertZeros(buffer, 1);
        FIR_Processor.processBlock(buffer, midi);

    }
    // 更新滤波器系数
    void updateCoefficients(const std::vector<float>& newCoeffs)
    {
        FIR_Processor.updateCoefficients(newCoeffs);
    }
private:


    FIRProcessor FIR_Processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Interpolator)
};



#endif //INTERPOLATOR_H
