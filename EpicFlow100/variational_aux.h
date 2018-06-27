#ifdef __cplusplus
extern "C" {
#endif

#ifndef __VARIATIONAL_AUX_H_
#define __VARIATIONAL_AUX_H_

#include <stdlib.h>
#include "image.h"

/* warp a color image according to a flow. src is the input image, wx and wy, the input flow. dst is the warped image and mask contains 0 or 1 if the pixels goes outside/inside image boundaries */
void ef_image_warp(ef_color_image_t *dst, ef_image_t *mask, const ef_color_image_t *src, const ef_image_t *wx,
                   const ef_image_t *wy);

/* compute image first and second order spatio-temporal derivatives of a color image */
void ef_get_derivatives(const ef_color_image_t *im1, const ef_color_image_t *im2, const convolution_t *deriv,
                        ef_color_image_t *dx, ef_color_image_t *dy, ef_color_image_t *dt, ef_color_image_t *dxx,
                        ef_color_image_t *dxy, ef_color_image_t *dyy, ef_color_image_t *dxt, ef_color_image_t *dyt);

/* compute the smoothness term */
void ef_compute_smoothness(ef_image_t *dst_horiz, ef_image_t *dst_vert, const ef_image_t *uu, const ef_image_t *vv,
                           const ef_image_t *dpsis_weight, const convolution_t *deriv_flow, const float half_alpha);

/* sub the laplacian (smoothness term) to the right-hand term */
void ef_sub_laplacian(ef_image_t *dst, const ef_image_t *src, const ef_image_t *weight_horiz,
                      const ef_image_t *weight_vert);

/* compute local smoothness weight as a sigmoid on image gradient*/
ef_image_t* ef_compute_dpsis_weight(ef_color_image_t *im, float coef, const convolution_t *deriv);

/* compute the dataterm and the matching term
   a11 a12 a22 represents the 2x2 diagonal matrix, b1 and b2 the right hand side
   other (color) images are input */
void ef_compute_data_and_match(ef_image_t *a11, ef_image_t *a12, ef_image_t *a22, ef_image_t *b1, ef_image_t *b2,
                               ef_image_t *mask, ef_image_t *du, ef_image_t *dv, ef_color_image_t *Ix,
                               ef_color_image_t *Iy, ef_color_image_t *Iz, ef_color_image_t *Ixx, ef_color_image_t *Ixy,
                               ef_color_image_t *Iyy, ef_color_image_t *Ixz, ef_color_image_t *Iyz,
                               const float half_delta_over3, const float half_gamma_over3);

#endif

#ifdef __cplusplus
}
#endif
