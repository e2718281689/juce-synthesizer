#include "common.h"
#include "table.h"
#include "math.h"

#define MAP_VALUE(input, input_min, input_max, output_min, output_max) \
    (output_min + ((input - input_min) / (input_max - input_min)) * (output_max - output_min))



float linear_interpolate(float x0, float y0, float x1, float y1, float x) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

inline float exp1(float x)
{
    x = 1.0 + x / 32.0;
    x *= x; x *= x; x *= x; x *= x;
    x *= x;

    // x *= x; x *= x; x *= x; x *= x;
    return x;
}

// inline float exp_2(double x)
// {
//    x = 1.0 + x / 1024;
//    x *= x; x *= x; x *= x; x *= x;
//    x *= x; x *= x; x *= x; x *= x;
//    x *= x; x *= x;
//    return x;
// }


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
    return (float)1 / ((float)1 + exp(-value));
    // if(value >= 5)
    // {
    //     return 1;
    // }
    // if(value <= -5)
    // {
    //     return 0;
    // }
    // if(value > 0)
    // {
    //     return sigmoid_values[(int)MAP_VALUE(value, 0.0, 5.0, 0.0, 8192.0)];
    // }
    // if(value == 0)
    // {
    //     return 0.5;
    // }
    // if(value < 0)
    // {
    //     return (1 - sigmoid_values[(int)MAP_VALUE((-value), 0.0, 5.0, 0.0, 8192.0)]);
    // }

    // return sigmoid_values[(int)MAP_VALUE(value, (-5.0), (5.0), (0.0), (8192.0))];

    // // 计算输入值在查表范围内的索引位置
    // value = MAP_VALUE(value, (-5.0), (5.0), (0.0), (4096.0));
    // int index = (int)value;
    // float lower_value = (float)index;
    // float upper_value = lower_value + 1.0;

    // // 获取相邻的两个预计算的 sigmoid 值
    // float y0 = sigmoid_values[index];
    // float y1 = sigmoid_values[index + 1];

    // // 线性插值
    // return linear_interpolate(lower_value, y0, upper_value, y1, value);
}

float tanh_approximate(float x)
{
	float x_poly = x * (1 + 0.183428244899 * x * x);
	float result = x_poly * (1 / sqrt(x_poly * x_poly + 1));
	return result;

    // return tanh_values[(int)MAP_VALUE(x, -5.0, 5.0, 0.0, 8192.0)];

    // if(x > 0)
    // {
    //     return tanh_values[(int)MAP_VALUE(x, 0.0, 5.0, 0.0, 8192.0)];
    // }
    // if(x == 0)
    // {
    //     return 0;
    // }
    // if(x < 0)
    // {
    //     return (- tanh_values[(int)MAP_VALUE((-x), 0.0, 5.0, 0.0, 8192.0)]);
    // }
}
