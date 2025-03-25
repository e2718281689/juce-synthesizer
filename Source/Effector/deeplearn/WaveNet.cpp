#include "WaveNet.h"
#include "model_data_nam.h"
#include "../wavenet/wavenet_layer.hpp"
#include "../cWaveNet/cDenseT.h"
#include "../cWaveNet/cConv1d.h"
#include "../cWaveNet/cWavenet_Layer.h"
#include "../cWaveNet/Matrix.h"

WaveNetProcessor::WaveNetProcessor()
{
    setupWeightsNam();
    setupWeightsNam();
    SelectModel();
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
  SelectModel();

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
void debug_cWavenet_Layer_forward(cWavenet_Layer *layer, Matrix *ins, Matrix *condition, Matrix *head_io)
{
        // conv.forward (ins);
        // input_mixin.forward (condition);
        // outs = conv.outs + input_mixin.outs;
        // activation.forward (outs);
        // head_io.noalias() += activation.outs;
        // _1x1.forward (activation.outs);
        // outs = ins + _1x1.outs;

        // conv.forward (ins);
        cconv1d_forward(&layer->conv, ins);
        
        // input_mixin.forward (condition);
        cDenseT_forward(&layer->input_mixin, condition);

        std::cout << "layer->conv.outs" << std::endl;
        printMatrix(&layer->conv.outs);

        std::cout << "layer->input_mixin.outs" << std::endl;
        printMatrix(&layer->input_mixin.outs);


        // outs = conv.outs + input_mixin.outs;
        matrixAdd(&layer->conv.outs, &layer->input_mixin.outs, &layer->outs);

        std::cout << "layer->conv.outs" << std::endl;
        printMatrix(&layer->conv.outs);


        // activation.forward (outs);
        TanhActivation_forward(&layer->activation, &layer->outs);

        std::cout << "layer->activation.outs_internal" << std::endl;
        printMatrix(&layer->activation.outs_internal);


        // head_io.noalias() += activation.outs;
        matrixAdd(head_io, &layer->activation.outs_internal, head_io);

        std::cout << "head_io" << std::endl;
        printMatrix(head_io);


        // _1x1.forward (activation.outs);
        cDenseT_forward(&layer->_1x1, &layer->activation.outs_internal);

        std::cout << "layer->_1x1.outs" << std::endl;
        printMatrix(&layer->_1x1.outs);


        // outs = ins + _1x1.outs;
        matrixAdd(ins, &layer->_1x1.outs, &layer->outs);

        std::cout << "layer->outs" << std::endl;
        printMatrix(&layer->outs);



}
void WaveNetProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &)
{
    _ASSERTE( _CrtCheckMemory( ) );
#if 1
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


#if 0

    std::cout << "c project" << std::endl;

    float *weights_p = model_collection_nam[0].weights.data();

    Matrix ins;
    ins = createMatrix(1, 1);
    clearMatrix(&ins);
    setElement(&ins, 0, 0, 0.7f);

    cDenseT_Layer rechannel;
    cDenseT_init(&rechannel, 1, 2, 0);
    cDenseT_setWeights(&rechannel, weights_p);
    weights_p += rechannel.out_size * rechannel.in_size ;

    // cDenseT_setBias(&rechannel, weights_p);
    // weights_p += rechannel.out_size;



    Matrix head_input;
    head_input = createMatrix(2, 1);
    clearMatrix(&head_input);


    cWavenet_Layer layer_arrays;
    cWavenet_Layer_init(&layer_arrays, 1, 2, 3, 1);
    cWavenet_Layer_load_weights(&layer_arrays, weights_p);

    cDenseT_forward(&rechannel, &ins);

    std::cout << "rechannel.outs" << std::endl;
    printMatrix(&rechannel.outs);

    debug_cWavenet_Layer_forward(&layer_arrays, &rechannel.outs, &ins, &head_input);

    std::cout << "layer_arrays.outs" << std::endl;
    printMatrix(&layer_arrays.outs);
    





#endif 

#if 0
        
    std::cout << "c++ project" << std::endl;

    auto cp_weights_p = model_collection_nam[0].weights.begin();

    Eigen::Matrix<float, 1, 1> cp_ins;
    Eigen::Matrix<float, 2, 1> cp_head_io;
    RTNeural::DenseT<float, 1, 2, false> cp_rechannel;
    wavenet::Wavenet_Layer<float, 1, 2, 3, 1, NAMMathsProvider> cp_layer;

    cp_ins.setZero();
    cp_head_io.setZero();

    std::vector<std::vector<float>> rechannel_weights (2, std::vector<float> (1));
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 1; j++)
            rechannel_weights[i][j] = *(cp_weights_p++);

    cp_rechannel.setWeights (rechannel_weights);
    cp_layer.load_weights (cp_weights_p);

    cp_ins << 0.7f;

    cp_rechannel.forward (cp_ins);

    std::cout << "rechannel.outs" << std::endl;
    std::cout << cp_rechannel.outs<< std::endl;

    cp_layer.forward (cp_rechannel.outs, cp_ins, cp_head_io);


    std::cout << "cp_layer.outs" << std::endl;
    std::cout << cp_layer.outs << std::endl;
#endif

    _ASSERTE( _CrtCheckMemory( ) );
}