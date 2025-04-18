/*
  ==============================================================================

    filter.h
    Created: 9 May 2024 4:09:55pm
    Author:  cjia

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ProcessorBase.h"
#include "digit_distortion.h"
#include "decimator.h"
#include "interpolator.h"
#include "FIRProcessor.h"

class overdrive : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    overdrive()
    {

    }
    ~overdrive()
    {
        Apvts->removeParameterListener("GainSlider", this);
    }
    void init(juce::AudioProcessorValueTreeState *apvts) override
    {
        Apvts = apvts;
        Apvts->addParameterListener("GainSlider", this);

        decimator.updateCoefficients(newCoeffs_88200);
        interpolator.updateCoefficients(newCoeffs_88200);
        // FIR_Processor.updateCoefficients(newCoeffs);

    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {
        if (parameterID.equalsIgnoreCase("GainSlider"))
        {
            AudioEffectDigitDistortion1Configure(&DigitDistortion1_Unit,1,0.8,1,newValue,1);
        }
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        decimator.prepareToPlay(sampleRate, samplesPerBlock);
        interpolator.prepareToPlay(sampleRate, samplesPerBlock);
        FIR_Processor.prepareToPlay(sampleRate, samplesPerBlock);

        DigitDistortion1_Unit.enable = 1;
        DigitDistortion1_Unit.ct = &DigitDistortion1Con_text;
        DigitDistortion1_Unit.mix = 1;
        DigitDistortion1_Unit.low = 1;
        DigitDistortion1_Unit.high = 1;
        DigitDistortion1_Unit.gain = 3;
        DigitDistortion1_Unit.mode = 1;
        DigitDistortion1_Unit.sample_rate = sampleRate;
        AudioEffectDigitDistortion1Init(&DigitDistortion1_Unit,sampleRate);
        AudioEffectDigitDistortion1Configure(&DigitDistortion1_Unit,0,0.8,1,3,1);
    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi) override
    {

        //


        interpolator.processBlock(buffer, midi);

        float *buff_R = buffer.getWritePointer(0);
        float *buff_L = buffer.getWritePointer(1);
        uint32_t num = buffer.getNumSamples();
        AudioEffectDigitDistortion1Apply(&DigitDistortion1_Unit,buff_R,buff_R,num);
        memcpy(buff_L, buff_R, num * sizeof(float));

        decimator.processBlock(buffer, midi);


        // FIR_Processor.processBlock(buffer, midi);

    }

    void reset() override
    {
        
    }


    const juce::String getName() const override { return "overdrive"; }

private:

    DigitDistortion1Unit DigitDistortion1_Unit;
    DigitDistortion1Context DigitDistortion1Con_text;

    Decimator decimator;
    Interpolator interpolator;
    FIRProcessor FIR_Processor;
    juce::AudioProcessorValueTreeState *Apvts;

    const std::vector<float> newCoeffs = {
        3.78339813e-04,  3.36851754e-04,  2.91680218e-04,  2.39002630e-04,
        1.74263848e-04,  9.24572941e-05, -1.15409636e-05, -1.42597730e-04,
       -3.04922665e-04, -5.01659388e-04, -7.34485523e-04, -1.00324002e-03,
       -1.30559585e-03, -1.63679529e-03, -1.98946326e-03, -2.35351197e-03,
       -2.71614713e-03, -3.06198256e-03, -3.37326636e-03, -3.63021743e-03,
       -3.81146739e-03, -3.89459823e-03, -3.85676272e-03, -3.67537049e-03,
       -3.32881980e-03, -2.79725253e-03, -2.06330809e-03, -1.11285096e-03,
        6.43535219e-05,  1.47403975e-03,  3.11694179e-03,  4.98841319e-03,
        7.07818395e-03,  9.37026792e-03,  1.18430296e-02,  1.44694140e-02,
        1.72173393e-02,  2.00502443e-02,  2.29277811e-02,  2.58066356e-02,
        2.86414550e-02,  3.13858590e-02,  3.39935052e-02,  3.64191820e-02,
        3.86198946e-02,  4.05559153e-02,  4.21917667e-02,  4.34971089e-02,
        4.44475043e-02,  4.50250382e-02,  4.52187750e-02,  4.50250382e-02,
        4.44475043e-02,  4.34971089e-02,  4.21917667e-02,  4.05559153e-02,
        3.86198946e-02,  3.64191820e-02,  3.39935052e-02,  3.13858590e-02,
        2.86414550e-02,  2.58066356e-02,  2.29277811e-02,  2.00502443e-02,
        1.72173393e-02,  1.44694140e-02,  1.18430296e-02,  9.37026792e-03,
        7.07818395e-03,  4.98841319e-03,  3.11694179e-03,  1.47403975e-03,
        6.43535219e-05, -1.11285096e-03, -2.06330809e-03, -2.79725253e-03,
       -3.32881980e-03, -3.67537049e-03, -3.85676272e-03, -3.89459823e-03,
       -3.81146739e-03, -3.63021743e-03, -3.37326636e-03, -3.06198256e-03,
       -2.71614713e-03, -2.35351197e-03, -1.98946326e-03, -1.63679529e-03,
       -1.30559585e-03, -1.00324002e-03, -7.34485523e-04, -5.01659388e-04,
       -3.04922665e-04, -1.42597730e-04, -1.15409636e-05,  9.24572941e-05,
        1.74263848e-04,  2.39002630e-04,  2.91680218e-04,  3.36851754e-04,
        3.78339813e-04
       };

    const std::vector<float> newCoeffs_88200 =
        {
        -0.00033237,
         0.00016625,
         3.114e-05,
         -0.00026671,
         0.00054244,
         -0.00085041,
         0.001169,
         -0.00146115,
         0.00167562,
         -0.00175131,
         0.00162486,
         -0.00124067,
         0.00056224,
         0.00041649,
         -0.00166173,
         0.00309392,
         -0.0045868,
         0.00597246,
         -0.00705276,
         0.00761677,
         -0.00746301,
         0.0064247,
         -0.00439555,
         0.00135354,
         0.00262025,
         -0.00732976,
         0.01246098,
         -0.01758827,
         0.02218945,
         -0.02566723,
         0.02737267,
         -0.02662321,
         0.02270287,
         -0.01482035,
         0.00197253,
         0.01742651,
         -0.04660157,
         0.09404117,
         -0.19161442,
         0.62987144,
         0.62987144,
         -0.19161442,
         0.09404117,
         -0.04660157,
         0.01742651,
         0.00197253,
         -0.01482035,
         0.02270287,
         -0.02662321,
         0.02737267,
         -0.02566723,
         0.02218945,
         -0.01758827,
         0.01246098,
         -0.00732976,
         0.00262025,
         0.00135354,
         -0.00439555,
         0.0064247,
         -0.00746301,
         0.00761677,
         -0.00705276,
         0.00597246,
         -0.0045868,
         0.00309392,
         -0.00166173,
         0.00041649,
         0.00056224,
         -0.00124067,
         0.00162486,
         -0.00175131,
         0.00167562,
         -0.00146115,
         0.001169,
         -0.00085041,
         0.00054244,
         -0.00026671,
         3.114e-05,
         0.00016625,
         -0.00033237
         };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(overdrive)
};