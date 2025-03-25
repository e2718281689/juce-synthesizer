/*
  ==============================================================================

    RTGru.h
    Created: 13 Feb 2025 5:04:36pm
    Author:  maple

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <RTNeural/RTNeural.h>
#include "../ProcessorBase.h"
#include "../wavenet/wavenet_model.hpp"
#include "../cWaveNet/cWaveNet.h"
#include "../cWaveNet/Matrix.h"

struct NAMMathsProvider
{
#if RTNEURAL_USE_EIGEN
  template <typename Matrix>
  static auto tanh(const Matrix &x)
  {
    // See: math_approx::tanh<3>
    // const auto x_poly = x.array() * (1.0f + 0.183428244899f * x.array().square());
    // return x_poly.array() * (x_poly.array().square() + 1.0f).array().rsqrt();
    return x.array().tanh(); // Tried using Eigen's built in tanh(), also works, failed on the same larger models as above custom tanh
  }
#elif RTNEURAL_USE_XSIMD
  template <typename T>
  static T tanh(const T &x)
  {
    return math_approx::tanh<3>(x);
  }
#endif
};

class WaveNetProcessor : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
  WaveNetProcessor();
  ~WaveNetProcessor() {};
  void init(juce::AudioProcessorValueTreeState *apvts) override ;
  void parameterChanged(const juce::String &parameterID, float newValue) override;
  // void prepareToPlay(double sampleRate, int samplesPerBlock);
  void processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &);
  // void reset();

  void SetMod(int mod);
  void SelectModel();
  const juce::String getName() const override { return "WaveNet"; }

private:
  using Dilations = wavenet::Dilations<1, 2, 4, 8, 16, 32, 64>;
  using Dilations2 = wavenet::Dilations<128, 256, 512, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512>;
  wavenet::Wavenet_Model<float,
                         1,
                         wavenet::Layer_Array<float, 1, 1, 2, 2, 3, Dilations, false, NAMMathsProvider>,
                         wavenet::Layer_Array<float, 2, 1, 1, 2, 3, Dilations2, true, NAMMathsProvider>>
  rtneural_wavenet;

  unsigned int cORcPPindex = 0;
  int modelIndex = 0;
  int m_currentModelindex = -1;

  juce::AudioProcessorValueTreeState *Apvts;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveNetProcessor)
};