#include "cWavenet_Layer.h"

void cWavenet_Layer_init(cWavenet_Layer *layer, int condition_size, int channels, int kernel_size, int dilation)
{

    layer->condition_size = condition_size;
    layer->channels = channels;
    layer->kernel_size = kernel_size;
    layer->dilation = dilation;

    cconv1d_init(&layer->conv, channels, channels, kernel_size, dilation, 1);
    cDenseT_init(&layer->input_mixin, condition_size, channels, 0);
    cDenseT_init(&layer->_1x1, channels, channels, 1);

    TanhActivation_init(&layer->activation, channels);

    layer->outs = createMatrix(channels, 1);
    clearMatrix(&layer->outs);

}

// 释放内存
void cWavenet_Layer_free(cWavenet_Layer *layer)
{
    cconv1d_Free(&layer->conv);
    cDenseT_free(&layer->input_mixin);
    cDenseT_free(&layer->_1x1);
}

//轴对称反转卷积核 
void reverseKernels(float* conv_weights, int channels, int filters_per_group, int kernel_size)
{
    for (int i = 0; i < channels; ++i)
    {
        for (int j = 0; j < filters_per_group; ++j)
        {
            for (int k = 0; k < kernel_size / 2; ++k)
            {
                int left_index = i * (filters_per_group * kernel_size) + j * kernel_size + k;
                int right_index = i * (filters_per_group * kernel_size) + j * kernel_size + (kernel_size - k - 1);

                // 交换左右两端的元素
                float temp = conv_weights[left_index];
                conv_weights[left_index] = conv_weights[right_index];
                conv_weights[right_index] = temp;
            }
        }
    }
}

// 加载参数
void cWavenet_Layer_load_weights(cWavenet_Layer *layer, float** weights) 
{
    // 重置卷积层
    // resetConvLayer(conv);

    float* weights_p = *weights;


    int weights_len = layer->conv.out_size* layer->conv.filters_per_group* layer->conv.kernel_size;
    float *conv_weights = (float *)malloc(weights_len * sizeof(Matrix));

    for (int i = 0; i < weights_len; i++)
    {
        conv_weights[i] = *(weights_p++);
    }
    reverseKernels(conv_weights, layer->channels, layer->conv.filters_per_group, layer->kernel_size);

    // 设置卷积层的权重
    cconv1d_setWeights(&layer->conv, conv_weights);

    // weights_p += layer->channels * layer->channels * layer->kernel_size;

    // 设置卷积层的偏置
    cconv1d_setBias(&layer->conv, weights_p);

    weights_p += layer->channels;

    // 设置输入混合层的权重
    cDenseT_setWeights(&layer->input_mixin, weights_p );

    weights_p += (layer->channels * layer->condition_size);
    free(conv_weights);

    // 设置 1x1 层的权重
    cDenseT_setWeights(&layer->_1x1, weights_p);

    weights_p += (layer->channels * layer->channels);

    // 设置 1x1 层的偏置
    cDenseT_setBias(&layer->_1x1, weights_p);
    weights_p += (layer->channels);


    *weights = weights_p;
}

// 执行 tanh 激活操作
void cWavenet_Layer_activation_Tanh(const Matrix* input, Matrix* output) 
{
    if (input->rows != output->rows || input->cols != output->cols) 
    {
        printf("Matrix size mismatch in tanh operation!\n");
        return;
    }

    for (int i = 0; i < input->rows * input->cols; ++i) 
    {
        output->data[i] = tanhf(input->data[i]);
    }
}

// void forward (const Eigen::Matrix<T, channels, 1>& ins,
//     const Eigen::Matrix<T, condition_size, 1>& condition,
//     Eigen::Matrix<T, channels, 1>& head_io)
// 向前扩散
void cWavenet_Layer_forward(cWavenet_Layer *layer, Matrix *ins, Matrix *condition, Matrix *head_io)
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

        // outs = conv.outs + input_mixin.outs;
        matrixAdd(&layer->conv.outs, &layer->input_mixin.outs, &layer->outs);

        // activation.forward (outs);
        TanhActivation_forward(&layer->activation, &layer->outs);

        // head_io.noalias() += activation.outs;
        matrixAdd(head_io, &layer->activation.outs_internal, head_io);

        // _1x1.forward (activation.outs);
        cDenseT_forward(&layer->_1x1, &layer->activation.outs_internal);

        // outs = ins + _1x1.outs;
        matrixAdd(ins, &layer->_1x1.outs, &layer->outs);


}

