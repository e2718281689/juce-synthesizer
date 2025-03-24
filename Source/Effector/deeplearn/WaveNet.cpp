#include "WaveNet.h"
#include "model_data_nam.h"
#include "../cWaveNet/cDenseT.h"
#include "../cWaveNet/cConv1d.h"

WaveNetProcessor::WaveNetProcessor()
{
    // setupWeightsNam();
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

void debug_cconv1d_forward(cConv1d_Layer *layer, Matrix *ins)
{
    // state.col(state_ptr) = ins;
    setColumn(&layer->state, layer->state_ptr, ins);

    // std::cerr << "&layer->state" << std::endl;
    // printMatrix(&layer->state);

    for (int k = 0; k < (layer->kernel_size); ++k)
    {
        // state_ptrs[k] = (state_ptr + state_size - k * dilation_rate) % state_size;

        setElement(&layer->state_ptrs, k, 0, (float)(((layer->state_ptr) + (layer->state_size) - ( k * (layer->dilation_rate))) % (layer->state_size)));
    }

    // std::cout << "&layer->state_ptrs" << std::endl;
    // printMatrix(&layer->state_ptrs);

    for (int k = 0; k < (layer->kernel_length); ++k)
    {
        // state_cols.col(k) = state.col(state_ptrs(k));

        copyColumn((&layer->state_cols), k, (&layer->state), (int)getElement((&layer->state_ptrs), k, 0));

        // std::cout << "&layer->state_cols" << std::endl;
        // printMatrix(&layer->state_cols);

    }

    // std::cout << "&layer->state_cols" << std::endl;
    // printMatrix(&layer->state_cols);

    for (int i = 0; i < layer->out_size; ++i)
    {
        // outs(i) = state_cols.cwiseProduct(weights[i]).sum() + bias(i);
        float sum = cwiseProductAndSum((&layer->state_cols), &(layer->weights[i]));
        setElement((&layer->outs), i, 0, sum + getElement((&layer->bias), i, 0));
    }

    // std::cout << "&layer->outs" << std::endl;
    // printMatrix(&layer->outs);

    (layer->state_ptr) = ((layer->state_ptr) == (layer->state_size) - 1 ? 0 : (layer->state_ptr) + 1); // iterate state pointer forwards
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

    std::cout << " c++ project " << std::endl;

    RTNeural::Conv1DT<float, 3, 2, 3, 1> conv;

    std::vector<std::vector<std::vector<float>>> weights = {
        // 第一个输出通道
        {
            {0.1f, 0.2f, 0.3f}, // 第一个输入通道的卷积核
            {0.4f, 0.5f, 0.6f}, // 第二个输入通道的卷积核
            {0.7f, 0.8f, 0.9f}  // 第三个输入通道的卷积核
        },
        // 第二个输出通道
        {
            {1.1f, 1.2f, 1.3f},
            {1.4f, 1.5f, 1.6f},
            {1.7f, 1.8f, 1.9f}
        }
    };
    
    conv.setWeights(weights);

    // 设置偏置 (out_size)
    // float bias[2] = {0.5f, -0.5f};

    std::vector<float> bias = {0.5f, -0.5f};

    conv.setBias(bias);

    std::cout << "weights:\n";
    std::cout << conv.weights[0] << std::endl;
    std::cout << conv.weights[1] << std::endl;

    std::cout << "bias:\n";
    std::cout << conv.bias << std::endl;


    // 生成10组测试数据
    for (int i = 0; i < 10; ++i) 
    {
        Eigen::Matrix<float, 3, 1> ins;
        ins << 1.0f * (i + 1), 2.0f * (i + 1), 1.5f * (i + 1);  // 生成不同的输入数据

        // 前向传播
        conv.forward(ins);

        // 输出结果
        std::cout << "outs:\n" << conv.outs << std::endl;
    }
#endif 



    std::cout << " c project " << std::endl;


#if 1

    cConv1d_Layer layer;
    cconv1d_init(&layer, 3, 2, 3, 1, 1);


    float cweights[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f };
    float cbias[] = { 0.5f, -0.5f };

    cconv1d_setWeights(&layer, cweights);
    cconv1d_setBias(&layer, cbias);

    std::cout << "weights:\n";
    printMatrix(&layer.weights[0]);
    printMatrix(&layer.weights[1]);

    std::cout << "bias:\n";
    printMatrix(&layer.bias);

    // 生成 10 组测试数据
    for (int i = 0; i < 10; ++i) 
    {
        // 创建矩阵并设置元素
        Matrix cins = createMatrix(3, 1);
        setElement(&cins, 0, 0, 1.0f * (i + 1));  // 第一个元素
        setElement(&cins, 1, 0, 2.0f * (i + 1));  // 第二个元素
        setElement(&cins, 2, 0, 1.5f * (i + 1));  // 第三个元素

        cconv1d_forward(&layer, &cins);

        std::cout << "outs:\n";
        printMatrix(&layer.outs);
    }

#endif 

    _ASSERTE( _CrtCheckMemory( ) );
}