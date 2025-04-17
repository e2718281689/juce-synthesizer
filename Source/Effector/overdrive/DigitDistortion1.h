/*
 * DigitDistortion1distortion.h
 * original web : https://github.com/JanosGit/Schrammel_OJD/tree/master
 * Created on: Apr 7, 2025
 * Author: syphen liang
 */

#ifndef _USER_DigitDistortion1_H_
#define _USER_DigitDistortion1_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

//biquad�˲����ṹ��
typedef struct {
    float a0, a1, a2;
    float b1, b2;
    float x1, x2;
    float y1, y2;
}BiFilter;

//��������
enum
{
	soft = 0,
	hard = 1
};

typedef struct _DigitDistortion1Context
{
	uint32_t 		sample_rate;
//	uint16_t   		channel;
	BiFilter 	Lp;
	BiFilter 	Hp;
}DigitDistortion1Context;

typedef struct _DigitDistortion1Unit
{
	DigitDistortion1Context 	   *ct;
	uint16_t 	       enable;//Ч��������־
	float			   mix;//��ʪ������,Խ��ʪ��Խ�󣬷�Χ:0~1
	float		   	   low;//��Ƶ,Խ���ƵԽ�࣬��Χ:0~1
	float		  	   high;//��Ƶ,Խ���ƵԽ�࣬��Χ:0~1
	float			   gain;//����
	int8_t			   mode;//ģʽѡ��,hardΪӲ������softΪ������
	uint32_t 		   sample_rate;
//	uint16_t		   channel;
}DigitDistortion1Unit;

void AudioEffectDigitDistortion1Apply(DigitDistortion1Unit *unit , float *pcm_in, float *pcm_out , uint32_t n);

void AudioEffectDigitDistortion1Init(DigitDistortion1Unit *unit ,uint32_t sample_rate);

void AudioEffectDigitDistortion1Configure(DigitDistortion1Unit *unit , float low ,float high,float mix,float gain,int8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* _USER_DigitDistortion1_H_ */
