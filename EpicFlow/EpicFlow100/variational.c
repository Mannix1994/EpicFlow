#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>

#include "variational.h"
#include "variational_aux.h"
#include "solver.h"


#include <xmmintrin.h>
typedef __v4sf v4sf;

convolution_t *deriv, *deriv_flow;
float half_alpha, half_delta_over3, half_gamma_over3;


/* perform flow computation at one level of the pyramid */
void compute_one_level(ef_image_t *wx, ef_image_t *wy, ef_color_image_t *im1, ef_color_image_t *im2, const ef_variational_params_t *params){
    const int width = wx->width, height = wx->height, stride=wx->stride;

    ef_image_t *du = ef_image_new(width, height), *dv = ef_image_new(width, height), // the flow increment
        *mask = ef_image_new(width, height), // mask containing 0 if a point goes outside image boundary, 1 otherwise
        *smooth_horiz = ef_image_new(width, height), *smooth_vert = ef_image_new(width, height), // horiz: (i,j) contains the diffusivity coeff from (i,j) to (i+1,j)
        *uu = ef_image_new(width, height), *vv = ef_image_new(width, height), // flow plus flow increment
        *a11 = ef_image_new(width, height), *a12 = ef_image_new(width, height), *a22 = ef_image_new(width, height), // system matrix A of Ax=b for each pixel
        *b1 = ef_image_new(width, height), *b2 = ef_image_new(width, height); // system matrix b of Ax=b for each pixel

    ef_color_image_t *w_im2 = ef_color_image_new(width, height), // warped second image
        *Ix = ef_color_image_new(width, height), *Iy = ef_color_image_new(width, height), *Iz = ef_color_image_new(width,
                                                                                                           height), // first order derivatives
        *Ixx = ef_color_image_new(width, height), *Ixy = ef_color_image_new(width, height), *Iyy = ef_color_image_new(width,
                                                                                                              height), *Ixz = ef_color_image_new(
            width, height), *Iyz = ef_color_image_new(
            width, height); // second order derivatives
  
  
    ef_image_t *dpsis_weight = ef_compute_dpsis_weight(im1, 5.0f, deriv);
  
    int i_outer_iteration;
    for(i_outer_iteration = 0 ; i_outer_iteration < params->niter_outer ; i_outer_iteration++){
        int i_inner_iteration;
        // warp second image
        ef_image_warp(w_im2, mask, im2, wx, wy);
        // compute derivatives
        ef_get_derivatives(im1, w_im2, deriv, Ix, Iy, Iz, Ixx, Ixy, Iyy, Ixz, Iyz);
        // erase du and dv
        ef_image_erase(du);
        ef_image_erase(dv);
        // initialize uu and vv
        memcpy(uu->data,wx->data,wx->stride*wx->height*sizeof(float));
        memcpy(vv->data,wy->data,wy->stride*wy->height*sizeof(float));
        // inner fixed point iterations
        for(i_inner_iteration = 0 ; i_inner_iteration < params->niter_inner ; i_inner_iteration++){
            //  compute robust function and system
            ef_compute_smoothness(smooth_horiz, smooth_vert, uu, vv, dpsis_weight, deriv_flow, half_alpha);
            ef_compute_data_and_match(a11, a12, a22, b1, b2, mask, du, dv, Ix, Iy, Iz, Ixx, Ixy, Iyy, Ixz, Iyz,
                                      half_delta_over3, half_gamma_over3);
            ef_sub_laplacian(b1, wx, smooth_horiz, smooth_vert);
            ef_sub_laplacian(b2, wy, smooth_horiz, smooth_vert);
            // solve system
            sor_coupled(du, dv, a11, a12, a22, b1, b2, smooth_horiz, smooth_vert, params->niter_solver, params->sor_omega);          
            // update flow plus flow increment
            int i;
            v4sf *uup = (v4sf*) uu->data, *vvp = (v4sf*) vv->data, *wxp = (v4sf*) wx->data, *wyp = (v4sf*) wy->data, *dup = (v4sf*) du->data, *dvp = (v4sf*) dv->data;
            for( i=0 ; i<height*stride/4 ; i++){
                (*uup) = (*wxp) + (*dup);
                (*vvp) = (*wyp) + (*dvp);
                uup+=1; vvp+=1; wxp+=1; wyp+=1;dup+=1;dvp+=1;
	        }
        }
        // add flow increment to current flow
        memcpy(wx->data,uu->data,uu->stride*uu->height*sizeof(float));
        memcpy(wy->data,vv->data,vv->stride*vv->height*sizeof(float));
    }   
    // free memory
    ef_image_delete(du);
    ef_image_delete(dv);
    ef_image_delete(mask);
    ef_image_delete(smooth_horiz);
    ef_image_delete(smooth_vert);
    ef_image_delete(uu);
    ef_image_delete(vv);
    ef_image_delete(a11);
    ef_image_delete(a12);
    ef_image_delete(a22);
    ef_image_delete(b1);
    ef_image_delete(b2);
    ef_image_delete(dpsis_weight);
    ef_color_image_delete(w_im2);
    ef_color_image_delete(Ix);
    ef_color_image_delete(Iy);
    ef_color_image_delete(Iz);
    ef_color_image_delete(Ixx);
    ef_color_image_delete(Ixy);
    ef_color_image_delete(Iyy);
    ef_color_image_delete(Ixz);
    ef_color_image_delete(Iyz);
}

/* set flow parameters to default */
void ef_variational_params_default(ef_variational_params_t *params){
    if(!params){
        fprintf(stderr,"Error optical_flow_params_default: argument is null\n");
        exit(1);
    }
    params->alpha = 1.0f;
    params->gamma = 0.71f;
    params->delta = 0.0f;
    params->sigma = 1.00f;
    params->niter_outer = 5;
    params->niter_inner = 1;  
    params->niter_solver = 30;
    params->sor_omega = 1.9f;
}
  
/* Compute a refinement of the optical flow (wx and wy are modified) between im1 and im2 */
void ef_variational(ef_image_t *wx, ef_image_t *wy, const ef_color_image_t *im1, const ef_color_image_t *im2,
                    ef_variational_params_t *params){
  
    // Check parameters
    if(!params){
        params = (ef_variational_params_t*) malloc(sizeof(ef_variational_params_t));
        if(!params){
          fprintf(stderr,"error: not enough memory\n");
          exit(1);
        }
        ef_variational_params_default(params);
    }

    // initialize global variables
    half_alpha = 0.5f*params->alpha;
    half_gamma_over3 = params->gamma*0.5f/3.0f;
    half_delta_over3 = params->delta*0.5f/3.0f;
    
    float deriv_filter[3] = {0.0f, -8.0f/12.0f, 1.0f/12.0f};
    deriv = ef_convolution_new(2, deriv_filter, 0);
    float deriv_filter_flow[2] = {0.0f, -0.5f};
    deriv_flow = ef_convolution_new(1, deriv_filter_flow, 0);


    // presmooth images
    int width = im1->width, height = im1->height, filter_size;
    ef_color_image_t *smooth_im1 = ef_color_image_new(width, height), *smooth_im2 = ef_color_image_new(width, height);
    float *presmooth_filter = ef_gaussian_filter(params->sigma, &filter_size);
    convolution_t *presmoothing = ef_convolution_new(filter_size, presmooth_filter, 1);
    ef_color_image_convolve_hv(smooth_im1, im1, presmoothing, presmoothing);
    ef_color_image_convolve_hv(smooth_im2, im2, presmoothing, presmoothing);
    ef_convolution_delete(presmoothing);
    free(presmooth_filter);
    
    compute_one_level(wx, wy, smooth_im1, smooth_im2, params);
  
    // free memory
    ef_color_image_delete(smooth_im1);
    ef_color_image_delete(smooth_im2);
    ef_convolution_delete(deriv);
    ef_convolution_delete(deriv_flow);
}

