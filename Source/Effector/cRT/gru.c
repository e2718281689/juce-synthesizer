#include <stdint.h>
#include <math.h>
#include "gru.h"
#include "common.h"
#include "string.h"

// kernel weights
float Wr [GRU_OUT_SIZE][GRU_IN_SIZE] = {0};
float Wz [GRU_OUT_SIZE][GRU_IN_SIZE] = {0};
float Wh [GRU_OUT_SIZE][GRU_IN_SIZE] = {0};
float kernel_outs [GRU_OUT_SIZE];

// single-input kernel weights
float Wz_1 [GRU_OUT_SIZE] = {0};
float Wr_1 [GRU_OUT_SIZE] = {0};
float Wh_1 [GRU_OUT_SIZE] = {0};

// recurrent weights
float Uz [GRU_OUT_SIZE][GRU_OUT_SIZE] = {0};
float Ur [GRU_OUT_SIZE][GRU_OUT_SIZE] = {0};
float Uh [GRU_OUT_SIZE][GRU_OUT_SIZE] = {0};

// biases
float bz[GRU_OUT_SIZE] = {0};
float br[GRU_OUT_SIZE] = {0};
float bh0[GRU_OUT_SIZE] = {0};
float bh1[GRU_OUT_SIZE] = {0};

// intermediate vars
float zt [GRU_OUT_SIZE] = {0};
float rt [GRU_OUT_SIZE] = {0};
float ct [GRU_OUT_SIZE] = {0};
float ht [GRU_OUT_SIZE] = {0};

float GRU_outs[GRU_OUT_SIZE] = {0};

void Gru_reset()
{
    memset(Wr, 0, sizeof(Wr));
    memset(Wz, 0, sizeof(Wz));
    memset(Wh, 0, sizeof(Wh));
    memset(kernel_outs, 0, sizeof(kernel_outs));
    memset(Wz_1, 0, sizeof(Wz_1));
    memset(Wr_1, 0, sizeof(Wr_1));
    memset(Wh_1, 0, sizeof(Wh_1));
    memset(Uz, 0, sizeof(Uz));
    memset(Ur, 0, sizeof(Ur));
    memset(Uh, 0, sizeof(Uh));
    memset(bz, 0, sizeof(bz));
    memset(br, 0, sizeof(br));
    memset(bh0, 0, sizeof(bh0));
    memset(bh1, 0, sizeof(bh1));
    memset(zt, 0, sizeof(zt));
    memset(rt, 0, sizeof(rt));
    memset(ct, 0, sizeof(ct));
    memset(ht, 0, sizeof(ht));
    memset(GRU_outs, 0, sizeof(GRU_outs));
}


void kernel_mat_mul(float vec[GRU_IN_SIZE],  float mat[GRU_OUT_SIZE][GRU_IN_SIZE], float out[GRU_OUT_SIZE])
{
	int j = 0;
    for (j = 0; j < GRU_OUT_SIZE; ++j)
    {
        out[j] = inner_product(vec, &mat[j][0], GRU_IN_SIZE);  // 调用点积函数

    }
}

void recurrent_mat_mul(float vec[GRU_OUT_SIZE], float mat[GRU_OUT_SIZE][GRU_OUT_SIZE], float out[GRU_OUT_SIZE])
{
	int j = 0;
    for(j = 0; j < GRU_OUT_SIZE; ++j)
    {
    	out[j] = inner_product(vec, &mat[j][0], GRU_OUT_SIZE);  // 调用点积函数
//    	out[j] = std::inner_product(mat[j], mat[j] + GRU_OUT_SIZE, vec, (T)0);
    }

}

void computeOutput()
{
	int i = 0;
	for(i = 0; i < GRU_OUT_SIZE; ++i)
		GRU_outs[i] = ((float)1.0 - zt[i]) * ht[i] + zt[i] * GRU_outs[i];
}

float* GRU_forward(float ins[GRU_IN_SIZE])
{
	int i = 0;

	// compute zt
	recurrent_mat_mul(GRU_outs, Uz, zt);
//	kernel_mat_mul(ins, Wz, kernel_outs);
	for(i = 0; i < GRU_OUT_SIZE; ++i)
	{
//		zt[i] = sigmoid(zt[i] + bz[i] + kernel_outs[i]);
		zt[i] = sigmoid(zt[i] + bz[i] + (Wz_1[i] * ins[0]));
	}


	// compute rt
	recurrent_mat_mul(GRU_outs, Ur, rt);
//	kernel_mat_mul(ins, Wr, kernel_outs);
	for(i = 0; i < GRU_OUT_SIZE; ++i)
	{
//		rt[i] = sigmoid(rt[i] + br[i] + kernel_outs[i]);
		rt[i] = sigmoid(rt[i] + br[i] + (Wr_1[i] * ins[0]));
	}


	// compute h_hat
	recurrent_mat_mul(GRU_outs, Uh, ct);
//	kernel_mat_mul(ins, Wh, kernel_outs);
	for(i = 0; i < GRU_OUT_SIZE; ++i)
	{
//		ht[i] = tanhf(rt[i] * (ct[i] + bh1[i]) + bh0[i] + kernel_outs[i]);
		ht[i] = tanh_approximate(rt[i] * (ct[i] + bh1[i]) + bh0[i] + (Wh_1[i] * ins[0]));
	}
	computeOutput();
	return GRU_outs;
}



void setWVals(float wVals[][27])
{
	int i = 0;
	int j = 0;

    for(i = 0; i < GRU_IN_SIZE; ++i)
    {
        for(j = 0; j < GRU_OUT_SIZE; ++j)
        {
            Wz[j][i] = wVals[i][j];
            Wr[j][i] = wVals[i][j + GRU_OUT_SIZE];
            Wh[j][i] = wVals[i][j + 2 * GRU_OUT_SIZE];
        }
    }

    for(j = 0; j < GRU_OUT_SIZE; ++j)
    {
        Wz_1[j] = wVals[0][j];
        Wr_1[j] = wVals[0][j + GRU_OUT_SIZE];
        Wh_1[j] = wVals[0][j + 2 * GRU_OUT_SIZE];
    }
}

void setUVals(float uVals[][27])
{
	int i = 0;
	int j = 0;

    for(i = 0; i < GRU_OUT_SIZE; ++i)
    {
        for(j = 0; j < GRU_OUT_SIZE; ++j)
        {
            Uz[j][i] = uVals[i][j];
            Ur[j][i] = uVals[i][j + GRU_OUT_SIZE];
            Uh[j][i] = uVals[i][j + 2 * GRU_OUT_SIZE];
        }
    }
}

void setBVals(float bVals[][27])
{
	int k = 0;
    for(k = 0; k < GRU_OUT_SIZE; ++k)
    {
        bz[k] = bVals[0][k] + bVals[1][k];
        br[k] = bVals[0][k + GRU_OUT_SIZE] + bVals[1][k + GRU_OUT_SIZE];
        bh0[k] = bVals[0][k + 2 * GRU_OUT_SIZE];
        bh1[k] = bVals[1][k + 2 * GRU_OUT_SIZE];
    }
}
