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

RTGruProcessor::RTGruProcessor(juce::AudioProcessorValueTreeState* apvts)
{
  Apvts = apvts;
  Apvts->addParameterListener("ModIndexComboBox", this);
  setupWeights();

  // apvts->getParameter("ModIndex");
  int modindex = apvts->getParameterAsValue("ModIndexComboBox").getValue();
  juce::Logger::outputDebugString("ModIndex =" + juce::String(modindex));
  
  SetMod(modindex);

  DL_init();
  
}

void RTGruProcessor::SetMod(int mod)
{
  modelIndex = mod;

  auto& gru = (model).template get<0>();
  auto& dense = (model).template get<1>();
  gru.setWVals(model_collection[modelIndex].rec_weight_ih_l0);
  gru.setUVals(model_collection[modelIndex].rec_weight_hh_l0);
  gru.setBVals(model_collection[modelIndex].rec_bias);
  dense.setWeights(model_collection[modelIndex].lin_weight);
  dense.setBias(model_collection[modelIndex].lin_bias.data());
  model.reset();
}

void RTGruProcessor::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{

    float* Data_L = buffer.getWritePointer(0);
    float* Data_R = buffer.getWritePointer(1);

    int numSamples = buffer.getNumSamples();

    int numChannels = buffer.getNumChannels();

    float Data_LT = 0;
    float Data_RT = 0;
    float Data_mono = 0;
    for (int i = 0; i < numSamples; ++i)
    {
        Data_LT = model.forward(&Data_L[i]);
        // Data_RT = DLapply_forward(Data_L[i] * 5);
        // Data_mono = Data_L[i]+Data_R[i];

        Data_L[i] = Data_LT;
        Data_R[i] = Data_LT;
    }

    //ampOut = model.forward(input_arr) + input_arr[0];   // Run Model and add Skip Connection; CHANGE FROM v0.1, was calculating skip wrong, should have also added input gain, fixed here
    //ampOut *= nnLevelAdjust;

}