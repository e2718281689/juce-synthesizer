#include <stdint.h>
#include <math.h>
#include "dense.h"
#include "common.h"


#define weights_size (DENSE_OUT_SIZE * DENSE_IN_SIZE)

float bias[DENSE_OUT_SIZE] = {0};
float weights[weights_size] = {0};

float DenseT_outs[DENSE_OUT_SIZE];

float* DenseT_forward(float ins[DENSE_IN_SIZE])
{
	int i = 0;
    for(i = 0; i < DENSE_OUT_SIZE; ++i)
        DenseT_outs[i] = inner_product(ins, &weights[i * DENSE_IN_SIZE], DENSE_IN_SIZE) + bias[i];

    return DenseT_outs;
}

void setWeights(float newWeights[][9])
{
	int i = 0;
	int k = 0;
    for(i = 0; i < DENSE_OUT_SIZE; ++i)
    {
        for(k = 0; k < DENSE_IN_SIZE; ++k)
        {
        	int idx = i * DENSE_IN_SIZE + k;
            weights[idx] = newWeights[i][k];
        }
    }
}

void setBias(float b) 
{ 
    bias[0] = b;
}
