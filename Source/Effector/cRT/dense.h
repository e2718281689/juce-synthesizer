#ifndef _DENSE_H
#define _DENSE_H

#define DENSE_OUT_SIZE (1)
#define DENSE_IN_SIZE (9)

float* DenseT_forward(float ins[DENSE_IN_SIZE]);
void setWeights(float newWeights[][9]);
void setBias(float b);


#endif /* _DENSE_H */
