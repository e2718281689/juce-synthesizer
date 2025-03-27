#include "WaveNet.h"
#include "model_data_nam.h"
#include "../wavenet/wavenet_layer.hpp"
#include "../cWaveNet/cDenseT.h"
#include "../cWaveNet/cConv1d.h"
#include "../cWaveNet/cWavenet_Layer.h"
#include "../cWaveNet/Matrix.h"
#include "../cWaveNet/cWavenet_layer_array.h"

WaveNetProcessor::WaveNetProcessor()
{
    setupWeightsNam();
    // setupWeightsNam();
    // SelectModel();
}

void WaveNetProcessor::SetMod(int mod)
{
    rtneural_wavenet.load_weights (model_collection_nam[mod].weights);
    static constexpr size_t N = 1; // number of samples sent through model at once
    rtneural_wavenet.prepare (N); // This is needed, including this allowed the led to come on before freezing
    rtneural_wavenet.prewarm();  // Note: looks like this just sends some 0's through the model
}

void WaveNetProcessor::SelectModel()
{
    if (m_currentModelindex != modelIndex) {
        rtneural_wavenet.load_weights (model_collection_nam[modelIndex].weights);
        static constexpr size_t N = 1; // number of samples sent through model at once
        rtneural_wavenet.prepare (N); // This is needed, including this allowed the led to come on before freezing
        rtneural_wavenet.prewarm();  // Note: looks like this just sends some 0's through the model
        m_currentModelindex = modelIndex;
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
//   SelectModel();

  int cORcPPindex = apvts->getParameterAsValue("cORcPPComboBox").getValue();
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
    juce::Logger::outputDebugString("cORcPPindex =" + juce::String(newValue));

  }
}
void printMatrix(const Matrix* mat) 
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
    _ASSERTE( _CrtCheckMemory( ) );
#if 0
    float *Data_L = buffer.getWritePointer(0);
    float *Data_R = buffer.getWritePointer(1);

    int numSamples = buffer.getNumSamples();

    // int numChannels = buffer.getNumChannels();

    for (int i = 0; i < numSamples; ++i)
    {
        Data_L[i] = rtneural_wavenet.forward(Data_L[i]);
    }
    memcpy(Data_R, Data_L, numSamples * sizeof(float));
#endif 


#if 1

    std::cout << "c project" << std::endl;

    int c_Dilations[]={1, 2, 4, 8, 16, 32, 64};

    float *weights_p = model_collection_nam[0].weights.data();

    cWavenet_layer_array layer_arrays;

    cWavenet_layer_array_init(&layer_arrays, 1, 1, 2, 2, 3, c_Dilations, 7, 0);

    cWavenet_layer_array_load_weights(&layer_arrays, weights_p);

    Matrix ins = createMatrix(1, 1);
    setElement(&ins, 0, 0, 0.7f);

    Matrix head_input = createMatrix(2, 1);
    clearMatrix(&head_input);

    std::cout << "ins" << std::endl;
    printMatrix(&ins);

    std::cout << "head_input" << std::endl;
    printMatrix(&head_input);

    for (int i = 0; i < 10; i++)
    {
        setElement(&ins, 0, 0, i*0.1f);
        cWavenet_layer_array_forward(&layer_arrays, &ins, &ins, &head_input);

        std::cout << "this " + std::to_string(i) + "floor" << std::endl;
        std::cout << "layer_arrays.layer_outputs" << std::endl;
        printMatrix(&layer_arrays.layer_outputs);
    }



#endif 

#if 1
        
    std::cout << "c++ project" << std::endl;

    wavenet::Layer_Array<float, 1, 1, 2, 2, 3, Dilations, false, NAMMathsProvider> cp_layer_arrays;

    auto cp_weights_p = model_collection_nam[0].weights.begin();

    cp_layer_arrays.load_weights(cp_weights_p);

    Eigen::Matrix<float, 1, 1> cp_v_ins;
    cp_v_ins<< 0.7f;

    Eigen::Matrix<float, 2, 1> cp_head_input {};
    cp_head_input.setZero();

    std::cout << "cp_v_ins" << std::endl;
    std::cout << cp_v_ins<< std::endl;

    std::cout << "cp_head_input" << std::endl;
    std::cout << cp_head_input<< std::endl;

    for (int i = 0; i < 10; i++)
    {
        cp_v_ins<< i*0.1f;

        cp_layer_arrays.forward(cp_v_ins, cp_v_ins, cp_head_input);

        std::cout << "this " + std::to_string(i) + "floor" << std::endl;
        std::cout << "cp_layer_arrays.layer_outputs" << std::endl;
        std::cout << cp_layer_arrays.layer_outputs << std::endl;
    }


#endif

    _ASSERTE( _CrtCheckMemory( ) );
}