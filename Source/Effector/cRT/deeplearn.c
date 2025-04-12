#include <stdint.h>
#include <math.h>
#include "data_gru.h"
#include "gru.h"
#include "dense.h"
#include "deeplearn.h"


uint8_t DL_en = 0;

void DL_init(uint16_t mod)
{
	Gru_reset();
	DenseT_reset();

    setWVals(c_model_collection[mod]->rec_weight_ih_l0);
    setUVals(c_model_collection[mod]->rec_weight_hh_l0);
    setBVals(c_model_collection[mod]->rec_bias);
    setWeights(c_model_collection[mod]->lin_weight);
    setBias(*(c_model_collection[mod]->lin_bias));

    DL_en = 1;
    //APP_DBG("DL_init\r\n");
}

void DLapply(float* pcm_in, float* pcm_out, uint32_t n)
{
	uint16_t j =0;

	if(DL_en == 1)
	{
		for(j = 0; j < n; j++ )
		{
			pcm_out[j] = DLapply_forward(pcm_in[j] * 5);
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
