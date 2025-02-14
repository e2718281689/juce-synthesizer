#include "common.h"

inline float exp1(float x)
{
    x = 1.0 + x / 256.0;
    x *= x; x *= x; x *= x; x *= x;
    x *= x; x *= x; x *= x; x *= x;
    return x;
}

//inline float exp2(double x)
//{
//    x = 1.0 + x / 1024;
//    x *= x; x *= x; x *= x; x *= x;
//    x *= x; x *= x; x *= x; x *= x;
//    x *= x; x *= x;
//    return x;
//}


float inner_product(float *vec, float *row, int size) 
{
    float result = 0.0f;
    for (int i = 0; i < size; ++i)
    {
        result += vec[i] * row[i];
    }
    return result;
//
    //return nds32_dprod_f32(vec, row, size);
}

float sigmoid(float value)
{
    return (float)1 / ((float)1 + exp1(-value));
}

float tanh_approximate(float x)
{
	float x_poly = x * (1 + 0.183428244899 * x * x);
	float result = x_poly * (1 / sqrt(x_poly * x_poly + 1));
	return result;
}
