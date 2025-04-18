
#pragma once

#include <JuceHeader.h>
#include "../ProcessorBase.h"

class FIRProcessor : public ProcessorBase
{
public:
    // ... 其他必要的 AudioProcessor 方法
    FIRProcessor()
    {

    }
    ~FIRProcessor()
    {

    }
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = getTotalNumOutputChannels();
        
        // 准备滤波器
        firFilter.prepare(spec);
        firFilter.reset();
        
        // 设置初始系数
//        updateCoefficients({0.1f, 0.2f, 0.4f, 0.2f, 0.1f});
    }
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        firFilter.process(context);
    }
    
    // 更新滤波器系数
    void updateCoefficients(const std::vector<float>& newCoeffs)
    {
        auto newCoeffsPtr = juce::dsp::FIR::Coefficients<float>::Ptr(
                            new juce::dsp::FIR::Coefficients<float>(newCoeffs.data(), newCoeffs.size())
                            );
        
        *firFilter.state = *newCoeffsPtr;
    }

private:
    // 使用 ProcessorDuplicator 管理 FIR 滤波器
    juce::dsp::ProcessorDuplicator<
        juce::dsp::FIR::Filter<float>,
        juce::dsp::FIR::Coefficients<float>> firFilter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FIRProcessor)
};