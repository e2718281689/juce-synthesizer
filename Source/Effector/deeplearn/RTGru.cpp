/*
  ==============================================================================

    RTGru.cpp
    Created: 13 Feb 2025 5:04:16pm
    Author:  maple

  ==============================================================================
*/

#include "RTGru.h"
#include "all_model_data_gru9_4count.h"
#include "../cRT/deeplearn.h"

void RTGruProcessor::SetMod(int mod)
{
  modelIndex = mod;

  auto &gru = (model).template get<0>();
  auto &dense = (model).template get<1>();
  gru.setWVals(model_collection[modelIndex].rec_weight_ih_l0);
  gru.setUVals(model_collection[modelIndex].rec_weight_hh_l0);
  gru.setBVals(model_collection[modelIndex].rec_bias);
  dense.setWeights(model_collection[modelIndex].lin_weight);
  dense.setBias(model_collection[modelIndex].lin_bias.data());
  model.reset();
}
RTGruProcessor::~RTGruProcessor()
{
  Apvts->addParameterListener("ModIndexComboBox", this);
  Apvts->addParameterListener("cORcPPComboBox", this);
}

RTGruProcessor::RTGruProcessor()
{

}
void RTGruProcessor::init(juce::AudioProcessorValueTreeState *apvts)
{
  Apvts = apvts;
  Apvts->addParameterListener("ModIndexComboBox", this);
  Apvts->addParameterListener("cORcPPComboBox", this);
  setupWeights();

  // apvts->getParameter("ModIndex");
  int modindex = apvts->getParameterAsValue("ModIndexComboBox").getValue();
  juce::Logger::outputDebugString("ModIndex =" + juce::String(modindex));

  int cORcPPindex = apvts->getParameterAsValue("cORcPPComboBox").getValue();
  juce::Logger::outputDebugString("cORcPPindex =" + juce::String(cORcPPindex));

  SetMod(modindex);

  DL_init(modindex);
}
void RTGruProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
  if (parameterID.equalsIgnoreCase("ModIndexComboBox"))
  {
    // juce::Logger::outputDebugString("ModIndexComboBox");
    juce::Logger::outputDebugString("ModIndex =" + juce::String(newValue));
    SetMod((int)newValue);
    DL_init((int)newValue);
  }
  if (parameterID.equalsIgnoreCase("cORcPPComboBox"))
  {
    // juce::Logger::outputDebugString("ModIndexComboBox");
    juce::Logger::outputDebugString("cORcPPindex =" + juce::String(newValue));
    cORcPPindex = (int)newValue;
  }
}

void RTGruProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &)
{

  TRACE_DSP();

  float *Data_L = buffer.getWritePointer(0);
  float *Data_R = buffer.getWritePointer(1);

  int numSamples = buffer.getNumSamples();

  // int numChannels = buffer.getNumChannels();

  if (cORcPPindex == 1)
  {
    double timeSec;
    {
      ScopedTimeMeasurement m(timeSec);

      for (int i = 0; i < numSamples; ++i)
      {
        Data_L[i] = model.forward(&Data_L[i]);
      }
      memcpy(Data_R, Data_L, numSamples * sizeof(float));
    }
    // juce::Logger::outputDebugString("c++ GRU took " +  juce::String(timeSec));
  }

  if (cORcPPindex == 0)
  {
    double timeSec;
    {
      ScopedTimeMeasurement m(timeSec);

      DLapply(Data_L, Data_L, numSamples);
      memcpy(Data_R, Data_L, numSamples * sizeof(float));
    }
    // juce::Logger::outputDebugString("c GRU took " +  juce::String(timeSec));
  }
}