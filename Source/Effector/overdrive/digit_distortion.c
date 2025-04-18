#include "digit_Distortion.h"

#define M_PI 3.14159265358979323846

float hard_clip(float in)
{
    float out = in;

    float th = 1.0f/3.0f;

    const float in_abs = fabs(in);
    const float sign = copysign(1.0 ,in);

    if (in_abs < th)
        out = 2 * in;
    else
        out = sign * (3 - ((2 - 3 * in_abs) * (2 - 3 * in_abs))) / 3;

    if (in_abs > 2 * th)
        out = sign;

    return out;
}

float soft_clip(float in)
{
    return copysign(1.0 , in) * (1 - expf(-fabsf(in)));
}

float processBiquadFilter(BiFilter* bq, float x)
{
    float y = bq->a0 * x + bq->a1 * bq->x1 + bq->a2 * bq->x2 - bq->b1 * bq->y1 - bq->b2 * bq->y2;

    bq->x2 = bq->x1;
    bq->x1 = x;
    bq->y2 = bq->y1;
    bq->y1 = y;

    return y;
}

void configHighPassFilter(BiFilter *filter, float sample_rate, float cutoff , float Q)
{
	float K = tan( M_PI * cutoff / sample_rate ); // Q = 0.70710678;
	filter->b1 = 2 * Q * ( K * K - 1 ) / ( K * K * Q + K + Q );
	filter->b2 = ( K * K * Q - K + Q ) / ( K * K * Q + K + Q );
	filter->a0 = Q / ( K * K * Q + K + Q );
	filter->a1 = ( -2 * Q ) / ( K * K * Q + K + Q );
	filter->a2 = Q / ( K * K * Q + K + Q );
}

void configLowPassFilter(BiFilter *filter, float sample_rate, float cutoff , float Q)
{
	float K = tan(M_PI * cutoff / sample_rate);
	filter->b1 = 2 * Q * (K * K - 1) / (K * K * Q + K + Q);
	filter->b2 = (K * K * Q - K + Q) / (K * K * Q + K + Q);
	filter->a0 = (K * K * Q) / (K * K * Q + K + Q);
	filter->a1 = (2 * K * K * Q) / (K * K * Q + K + Q);
	filter->a2 = (K * K * Q) / (K * K * Q + K + Q);
}


void DigitDistortion1_Configure(DigitDistortion1Context* ct,float low ,float high)
{
	configHighPassFilter(&ct->Hp, ct->sample_rate, 50 + (1.0f - low) * 200 , 0.707);
	configLowPassFilter(&ct->Lp, ct->sample_rate, 3000 + high * 6000 , 0.707);
}
void AudioEffectDigitDistortion1Configure(DigitDistortion1Unit *unit , float low ,float high,float mix,float gain,int8_t mode)
{
	if(!unit->enable)
	{
		return;
	}

	unit->low  = low;
	unit->high = high;
	unit->mix  = mix;
	unit->gain = gain;
	unit->mode = mode;
	if(unit->ct != NULL)
	{
		DigitDistortion1_Configure(unit->ct,unit->low, unit->high);
	}
}

void DigitDistortion1_Init(DigitDistortion1Context* ct, uint8_t channel, uint32_t sample_rate)
{

}

void AudioEffectDigitDistortion1Init(DigitDistortion1Unit* unit, uint32_t sample_rate)
{
	if(!unit->enable)
	{
		return;
	}

	if(unit->ct == NULL)
	{
		// unit->ct = (DigitDistortion1Context *)osPortMallocFromEnd(sizeof(DigitDistortion1Context));
		if(unit->ct == NULL)
		{
			unit->enable = 0;
			// APP_DBG("DigitDistortion1Context mem malloc err! %ld\n",sizeof(DigitDistortion1Context));
		}
	}

	if(unit->ct != NULL)
	{
		memset(unit->ct, 0, sizeof(DigitDistortion1Context));//��ct��0

		unit->sample_rate = sample_rate;
		unit->ct->sample_rate = sample_rate;
	}
}


void DigitDistortion1_apply(DigitDistortion1Context* ct, float *pcm_in, float *pcm_out, uint32_t n,float low ,float high,float mix,float gain,int8_t mode)
{
	uint16_t j = 0;

	for(j = 0;j < n ;j++)
	{
		float input = pcm_in[j];


		float Hpout = processBiquadFilter(&ct->Hp , input);
#if 1

		float Pregainout = Hpout * gain * 18;


		float Shapeout = 0;
		if(mode == soft)
		{
			Shapeout = soft_clip(Pregainout) ;
		}else if(mode == hard)
		{
			Shapeout = hard_clip(Pregainout) ;
		}
		Shapeout = Shapeout * mix + (1 - mix) * input;
		float out = processBiquadFilter(&ct->Lp , Shapeout);

#endif

		pcm_out[j] = out ;
	}
}
void AudioEffectDigitDistortion1Apply(DigitDistortion1Unit *unit, float *pcm_in, float *pcm_out, uint32_t n)
{
	if((unit->enable) && (unit->ct != NULL))
	{
		DigitDistortion1_apply(unit->ct, pcm_in, pcm_out, n , unit->low, unit->high,  unit->mix, unit->gain, unit->mode);
	}
}

