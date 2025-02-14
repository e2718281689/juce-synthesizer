#include <stdint.h>
#include <math.h>
#include "data_gru.h"
#include "gru.h"
#include "dense.h"
#include "deeplearn.h"


uint8_t DL_en = 0;

void DL_init()
{
    setWVals(rec_weight_ih_l0);
    setUVals(rec_weight_hh_l0);
    setBVals(rec_bias);
    setWeights(lin_weight);
    setBias(*lin_bias);

    DL_en = 1;
    //APP_DBG("DL_init\r\n");
}


void DLapply(float* pcm_in, float* pcm_out, uint32_t n)
{
	int j =0;

	if(DL_en == 1)
	{
		for(j = 0; j < n; j++ )
		{
			float *GRU_out={0};
			GRU_out = GRU_forward(&pcm_in[2 * j + 1] );

			float *Dense_out={0};
			Dense_out = DenseT_forward(GRU_out);

			pcm_out[2 * j + 0] = *Dense_out;
			pcm_out[2 * j + 1] = *Dense_out;

		}

	}


}

float DLapply_forward(float pcm_in)
{
	float GRU_in = pcm_in;
	float GRU_out[GRU_OUT_SIZE] = {0};
	float *GRU_out_ptr = GRU_out;
	GRU_out_ptr = GRU_forward(&GRU_in);

	float *Dense_out = NULL;
	Dense_out = DenseT_forward(GRU_out_ptr);

	return *Dense_out;
}
