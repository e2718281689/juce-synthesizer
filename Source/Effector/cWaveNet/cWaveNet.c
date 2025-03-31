#include "cWaveNet.h"
#include "cWavenet_Layer.h"
#include "../cWaveNet/cWavenet_layer_array.h"

cWavenet_layer_array layer_arrays1;
cWavenet_layer_array layer_arrays2;

Matrix ins;
Matrix head_input;

float head_scale;
void WaveNet_init()
{

    int c_Dilations1[]={1, 2, 4, 8, 16, 32, 64};
    cWavenet_layer_array_init(&layer_arrays1, 1, 1, 2, 2, 3, c_Dilations1, 7, 0);

    
    int c_Dilations2[]={128, 256, 512, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
    cWavenet_layer_array_init(&layer_arrays2,  2, 1, 1, 2, 3, c_Dilations2, 13, 1);

    ins = createMatrix(1, 1);
    
    head_input = createMatrix(2, 1);
    clearMatrix(&head_input);
    

}

void WaveNet_load_weights(float *weights)
{
    cWavenet_layer_array_load_weights(&layer_arrays1, &weights);
    cWavenet_layer_array_load_weights(&layer_arrays2, &weights);
    
    head_scale = *weights++;
}
float WaveNet_forward(float pcm_in)
{

    setElement(&ins, 0, 0, pcm_in);

    clearMatrix(&head_input);

    cWavenet_layer_array_forward(&layer_arrays1, &ins, &ins, &head_input);

    // printf("&layer_arrays1.layer_outputs\n");
    // printMatrix(&layer_arrays1.layer_outputs);


    cWavenet_layer_array_forward(&layer_arrays2, &layer_arrays1.layer_outputs, &ins, &layer_arrays1.head_outputs);

    // printf("&layer_arrays1.head_outputs\n");
    // printMatrix(&layer_arrays1.head_outputs);

    return getElement(&layer_arrays2.head_outputs, 0, 0) * head_scale;

}
void WaveNet_reset()
{
    cWavenet_layer_array_reset(&layer_arrays1);
    cWavenet_layer_array_reset(&layer_arrays2);
}
void WaveNet_free()
{
    cWavenet_layer_array_free(&layer_arrays1);
    cWavenet_layer_array_free(&layer_arrays2);
    freeMatrix(&ins);
    freeMatrix(&head_input);
}