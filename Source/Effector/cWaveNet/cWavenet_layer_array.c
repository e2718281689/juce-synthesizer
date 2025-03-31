#include "cWavenet_layer_array.h"


void cWavenet_layer_array_init(cWavenet_layer_array *layer, 
                                int in_size, 
                                int condition_size, 
                                int head_size, 
                                int channels, 
                                int kernel_size, 
                                int* DilationsSequence,
                                int DilationsSum,
                                int has_head_bias)
{

    layer->DilationsSum = DilationsSum;
    layer->has_head_bias = has_head_bias;
    layer->in_size = in_size;
    layer->condition_size = condition_size;
    layer->head_size = head_size;
    layer->channels = channels;

    layer->head_outputs = createMatrix(head_size, 1);
    clearMatrix(&layer->head_outputs);



    layer->layers = (cWavenet_Layer*)malloc(DilationsSum * sizeof(cWavenet_Layer));

    for (int i = 0; i < DilationsSum; i++)
    {
        cWavenet_Layer_init(&layer->layers[i], condition_size, channels, kernel_size, DilationsSequence[i]);
    }

    layer->layer_outputs = createMatrix(layer->layers[0].outs.rows, layer->layers[0].outs.cols);
    clearMatrix(&layer->layer_outputs);


    cDenseT_init(&layer->rechannel, layer->in_size, layer->channels, 0);
    cDenseT_init(&layer->head_rechannel, layer->channels, layer->head_size, layer->has_head_bias);

}

void cWavenet_layer_array_load_weights(cWavenet_layer_array *layer ,float** weights )
{

    // std::vector<std::vector<float>> rechannel_weights (channels, std::vector<float> (in_size));
    // for (int i = 0; i < channels; i++)
    //     for (int j = 0; j < in_size; j++)
    //         rechannel_weights[i][j] = *(weights++);
    // rechannel.setWeights (rechannel_weights);

    // RTNeural::modelt_detail::forEachInTuple ([&weights] (auto& layer, size_t)
    //                                         { 
    //                                             layer.load_weights (weights); 
    //                                         },
    //                                          layers);

    // std::vector<std::vector<float>> head_rechannel_weights (head_size, std::vector<float> (channels));
    // for (int i = 0; i < head_size; i++)
    //     for (int j = 0; j < channels; j++)
    //         head_rechannel_weights[i][j] = *(weights++);
    // head_rechannel.setWeights (head_rechannel_weights);

    // if constexpr (has_head_bias)
    // {
    //     std::vector<float> head_rechannel_bias (head_size);
    //     for (int i = 0; i < head_size; i++)
    //         head_rechannel_bias[i] = *(weights++);
    //     head_rechannel.setBias (head_rechannel_bias.data());
    // }

    float* p_weights = *weights;

    cDenseT_setWeights(&layer->rechannel, p_weights);
    p_weights += layer->channels * layer->in_size ;

    for (int i = 0; i < layer->DilationsSum; i++)
    {
        cWavenet_Layer_load_weights(&layer->layers[i], &p_weights);
    }

    cDenseT_setWeights(&layer->head_rechannel, p_weights);
    p_weights += layer->head_size * layer->channels;

    if (layer->has_head_bias)
    {
        cDenseT_setBias(&layer->head_rechannel, p_weights);
        p_weights += layer->head_size;
    }

    *weights = p_weights;
}

// void forward (const Eigen::Matrix<T, in_size, 1>& ins,
//     const Eigen::Matrix<T, condition_size, 1>& condition,
//     Eigen::Matrix<T, channels, 1>& head_io)

void cWavenet_layer_array_forward(cWavenet_layer_array* layer, Matrix* ins, Matrix* condition, Matrix* head_io)
{
    // rechannel.forward (ins);
    cDenseT_forward(&layer->rechannel, ins);

    // RTNeural::modelt_detail::forEachInTuple (
    //     [&] (auto& layer, auto index_t)
    //     {
    //         static constexpr size_t index = index_t;
    //         if constexpr (index == 0)
    //             layer.forward (rechannel.outs, condition, head_io);
    //         else
    //             layer.forward (std::get<index - 1> (layers).outs, condition, head_io);
    //     },
    //     layers);
        
    for (int i = 0; i < layer->DilationsSum; i++)
    {

        if( i == 0 )
        {
            cWavenet_Layer_forward(&layer->layers[i], &layer->rechannel.outs, condition, head_io);
        }
        else
        {
            cWavenet_Layer_forward(&layer->layers[i], &layer->layers[i-1].outs, condition, head_io);
        }

    }
        

    //head_rechannel.forward (head_io);
    cDenseT_forward(&layer->head_rechannel, head_io);

    // head_outputs = head_rechannel.outs;
    copyMatrix(&layer->head_rechannel.outs, &layer->head_outputs);

    // decltype (Last_Layer_Type::outs)& layer_outputs { std::get<std::tuple_size_v<decltype (layers)> - 1> (layers).outs };
    copyMatrix(&layer->layers[layer->DilationsSum-1].outs, &layer->layer_outputs);
    
}
void cWavenet_layer_array_reset(cWavenet_layer_array* layer)
{
    for (int i = 0; i < layer->DilationsSum; i++)
    {
        cWavenet_Layer_reset(&layer->layers[i]);
    }
}
void cWavenet_layer_array_free(cWavenet_layer_array* layer)
{
    freeMatrix(&layer->head_outputs);
    for (int i = 0; i < layer->DilationsSum; i++)
    {
        cWavenet_Layer_free(&layer->layers[i]);
    }
    free(layer->layers);
    cDenseT_free(&layer->rechannel);
    cDenseT_free(&layer->head_rechannel);
}