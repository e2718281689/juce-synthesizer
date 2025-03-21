#include "cWaveNet.h"

float WaveNet_forward(float pcm_in)
{
    Matrix m1 = createMatrix(4, 3);

    Matrix m2 = createMatrix(4, 1);

    for (int i = 0; i < m1.rows * m1.cols; ++i) {
        m1.data[i] = 0.0f;
    }

    for (int i = 0; i < m2.rows * m2.cols; ++i) {
        m1.data[i] = i;
    }

    setColumn(&m1, 2, &m2);

    // printMatrix(&m1);

}