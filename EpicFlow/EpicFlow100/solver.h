#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "image.h"

// Perform n iterations of the sor_coupled algorithm for a system of the form as described in opticalflow.c
void sor_coupled(ef_image_t *du, ef_image_t *dv, ef_image_t *a11, ef_image_t *a12, ef_image_t *a22, ef_image_t *b1, ef_image_t *b2, ef_image_t *dpsis_horiz, ef_image_t *dpsis_vert, const int iterations, const float omega);

#ifdef __cplusplus
}
#endif

