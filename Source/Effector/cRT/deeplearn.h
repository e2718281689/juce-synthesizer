#ifndef _DEEP_LEARN_H
#define _DEEP_LEARN_H


#ifdef __cplusplus
extern "C" {
#endif

void DL_init();
void DLapply(float* pcm_in, float* pcm_out, uint32_t n);
float DLapply_forward(float pcm_in);

#ifdef __cplusplus
}
#endif

#endif /* _DEEP_LEARN_H */
