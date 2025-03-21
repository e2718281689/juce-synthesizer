#include "WaveNet.h"
#include "model_data_nam.h"
#include "../cWaveNet/cDenseT.h"

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
            std::cerr << juce::String(i) + "," + juce::String(j) + "=" + juce::String(getElement(mat, i, j)) + " " ; 
        }
        std::cerr << "" << std::endl;
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

#if 0
    RTNeural::DenseT<float, 3, 2, true> rechannel; // no bias!

    rechannel.setWeights({{0.1f, 0.2f, 0.3f},
                            { 0.4f, 0.5f, 0.6f}});

    float bias[2] = {0.7f, 0.8f};
    rechannel.setBias(bias);

    std::cout << "Weights: \n" << rechannel.weights << std::endl;

    Eigen::Matrix<float, 3, 1> ins;

    ins(0) = 1.0f;
    ins(1) = 2.0f;
    ins(2) = 3.0f;


    rechannel.forward(ins);

    std::cout << "outs: \n" << rechannel.outs << std::endl;
#endif 

#if 0
    cDenseT_Layer layer;
    cDenseT_init(&layer, 3, 2, 1); // in_size = 3, out_size = 2, has_bias = 1

    // 定义权重和偏置（行优先存储）
    float weights[] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f};
    // float bias[] = {0.7f, 0.8f};

    // 定义输入
    float input_data[] = {1.0f, 2.0f, 3.0f};
    Matrix input = createMatrix(3, 1);
    for (int i = 0; i < 3; ++i) 
        setElement(&input, i, 0, input_data[i]);

    // 设置权重和偏置
    cDenseT_setWeights(&layer, weights);
    cDenseT_setBias(&layer, bias);

    std::cout <<" layer.weights \n";
    printMatrix(&layer.weights);
    

    // 前向传播
    cDenseT_forward(&layer, &input);

    // 输出结果
    std::cout <<"Output:\n";
    printMatrix(&layer.outs);

    // 释放内存
    cDenseT_free(&layer);
    freeMatrix(&input);
#endif 

    _ASSERTE( _CrtCheckMemory( ) );
}