#include "WaveNet.h"
#include "model_data_nam.h"
#include "../wavenet/wavenet_layer.hpp"
#include "../cWaveNet/cDenseT.h"
#include "../cWaveNet/cConv1d.h"
#include "../cWaveNet/cWavenet_Layer.h"
#include "../cWaveNet/Matrix.h"
#include "../cWaveNet/cWavenet_layer_array.h"
#include "../cWaveNet/cWaveNet.h"

WaveNetProcessor::WaveNetProcessor()
{
    setupWeightsNam();
    setupWeightsNam();
    SelectModel();

    WaveNet_init();
}

void WaveNetProcessor::SetMod(int mod)
{
    rtneural_wavenet.load_weights (model_collection_nam[mod].weights);
    // static constexpr size_t N = 1; // number of samples sent through model at once
    // rtneural_wavenet.prepare (N); // This is needed, including this allowed the led to come on before freezing
    // rtneural_wavenet.prewarm();  // Note: looks like this just sends some 0's through the model

    WaveNet_load_weights(model_collection_nam[mod].weights.data());
}

void WaveNetProcessor::SelectModel()
{
    if (m_currentModelindex != modelIndex) {
        rtneural_wavenet.load_weights (model_collection_nam[modelIndex].weights);
        // static constexpr size_t N = 1; // number of samples sent through model at once
        // rtneural_wavenet.prepare (N); // This is needed, including this allowed the led to come on before freezing
        // rtneural_wavenet.prewarm();  // Note: looks like this just sends some 0's through the model
        m_currentModelindex = modelIndex;

        WaveNet_init();
        WaveNet_load_weights(model_collection_nam[modelIndex].weights.data());
        WaveNet_reset();
    }

}

void WaveNetProcessor::init(juce::AudioProcessorValueTreeState *apvts)
{
  Apvts = apvts;
  Apvts->addParameterListener("ModIndexComboBox", this);
  Apvts->addParameterListener("cORcPPComboBox", this);

  int LocalModelIndex = apvts->getParameterAsValue("ModIndexComboBox").getValue();
  juce::Logger::outputDebugString("ModIndex =" + juce::String(LocalModelIndex));
  
  modelIndex = LocalModelIndex;
  SelectModel();

  cORcPPindex = apvts->getParameterAsValue("cORcPPComboBox").getValue();
  juce::Logger::outputDebugString("cORcPPindex =" + juce::String(cORcPPindex));
}

void WaveNetProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
  if (parameterID.equalsIgnoreCase("ModIndexComboBox"))
  {
    // juce::Logger::outputDebugString("ModIndexComboBox");
    juce::Logger::outputDebugString("ModIndex =" + juce::String(newValue));
    modelIndex = newValue;
    SelectModel();

  }
  if (parameterID.equalsIgnoreCase("cORcPPComboBox"))   
  {
    // juce::Logger::outputDebugString("ModIndexComboBox");
    cORcPPindex = (int)newValue;
    juce::Logger::outputDebugString("cORcPPindex =" + juce::String(newValue));

  }
}
void cp_printMatrix(const Matrix* mat) 
{
    for (int i = 0; i < mat->rows; ++i) 
    {
        for (int j = 0; j < mat->cols; ++j) {
            // std::cout << juce::String(i) + "," + juce::String(j) + "=" + juce::String(getElement(mat, i, j)) + " " ; 
            std::cout << juce::String(getElement(mat, i, j)) + " " ; 
        }
        std::cout << "" << std::endl;
    }
}

void WaveNetProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &)
{
#if 1
    float *Data_L = buffer.getWritePointer(0);
    float *Data_R = buffer.getWritePointer(1);

    int numSamples = buffer.getNumSamples();

    // int numChannels = buffer.getNumChannels();

    for (int i = 0; i < numSamples; ++i)
    {
        if(cORcPPindex == 1)
        {
            Data_L[i] = rtneural_wavenet.forward(Data_L[i]);
        }

        if (cORcPPindex == 0)
        {
            Data_L[i] = WaveNet_forward(Data_L[i]);
        }

    }
    memcpy(Data_R, Data_L, numSamples * sizeof(float));
#endif 

    // WaveNet_init();
    // WaveNet_load_weights(model_collection_nam[0].weights.data());
    // WaveNet_reset();

    // for (int i = 0; i < (1 << 14); ++i)
    // {
    //     WaveNet_forward (0.0f);
    //     std::cout << "WaveNet_forward" << std::endl;
    // }
        

    // float c_in =- 0.5f;
    // float c_out = WaveNet_forward(c_in);


    // std::cout << "c_out" << std::endl;
    // std::cout << c_out << std::endl;




    // rtneural_wavenet.load_weights (model_collection_nam[0].weights);
    // static constexpr size_t N = 1; // number of samples sent through model at once
    // rtneural_wavenet.prepare (N); // This is needed, including this allowed the led to come on before freezing
    // rtneural_wavenet.prewarm();  // Note: looks like this just sends some 0's through the model
    

    // float cp_in =- 0.5f;
    // float cp_out = rtneural_wavenet.forward(cp_in);

    // std::cout << "cp_out" << std::endl;
    // std::cout << cp_out << std::endl;

}