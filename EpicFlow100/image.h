#ifdef __cplusplus
extern "C" {
#endif

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <stdio.h>

#define MINMAX(a,b) MIN( MAX(a,0) , b-1 )

/********** STRUCTURES *********/

/* structure for 1-channel image */
typedef struct ef_image_s
{
  int width;		/* Width of the image */
  int height;		/* Height of the image */
  int stride;		/* Width of the memory (width + paddind such that it is a multiple of 4) */
  float *data;		/* Image data, aligned */
} ef_image_t;

/* structure for 3-channels image stored with one layer per color, it assumes that c2 = c1+width*height and c3 = c2+width*height. */
typedef struct ef_color_image_s
{
    int width;			/* Width of the image */
    int height;			/* Height of the image */
    int stride;         /* Width of the memory (width + paddind such that it is a multiple of 4) */
    float *c1;			/* Color 1, aligned */
    float *c2;			/* Color 2, consecutive to c1*/
    float *c3;			/* Color 3, consecutive to c2 */
} ef_color_image_t;


/* structure for convolutions */
typedef struct convolution_s
{
    int order;			/* Order of the convolution */
    float *coeffs;		/* Coefficients */
    float *coeffs_accu;	/* Accumulated coefficients */
} convolution_t;

/********** Create/Delete **********/

/* allocate a new image of size width x height */
ef_image_t *ef_image_new(const int width, const int height);

/* allocate a new image and copy the content from src */
ef_image_t *ef_image_cpy(const ef_image_t *src);

/* set all pixels values to zeros */
void ef_image_erase(ef_image_t *image);

/* free memory of an image */
void ef_image_delete(ef_image_t *image);

/* multiply an image by a scalar */
void ef_image_mul_scalar(ef_image_t *image, const float scalar);

/* allocate a new color image of size width x height */
ef_color_image_t *ef_color_image_new(const int width, const int height);

/* allocate a new color image and copy the content from src */
ef_color_image_t *ef_color_image_cpy(const ef_color_image_t *src);

/* set all pixels values to zeros */
void ef_color_image_erase(ef_color_image_t *image);

/* free memory of a color image */
void ef_color_image_delete(ef_color_image_t *image);

/************ Convolution ******/

/* return half coefficient of a gaussian filter */
float *ef_gaussian_filter(const float sigma, int *fSize);

/* create a convolution structure with a given order, half_coeffs, symmetric or anti-symmetric according to even parameter */
convolution_t *ef_convolution_new(int order, const float *half_coeffs, const int even);

/* perform an horizontal convolution of an image */
void ef_convolve_horiz(ef_image_t *dest, const ef_image_t *src, const convolution_t *conv);

/* perform a vertical convolution of an image */
void ef_convolve_vert(ef_image_t *dest, const ef_image_t *src, const convolution_t *conv);

/* free memory of a convolution structure */
void ef_convolution_delete(convolution_t *conv);

/* perform horizontal and/or vertical convolution to a color image */
void ef_color_image_convolve_hv(ef_color_image_t *dst, const ef_color_image_t *src, const convolution_t *horiz_conv,
                                const convolution_t *vert_conv);

/************ Others **********/

/* return a new image in lab color space */
ef_color_image_t *ef_rgb_to_lab(const ef_color_image_t *im);

/* compute the saliency of a given image */
ef_image_t* ef_saliency(const ef_color_image_t *im, float sigma_image, float sigma_matrix);

#endif

#ifdef __cplusplus
}
#endif
