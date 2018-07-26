#include "image.h"
#include "std.h"


/********** Create/Delete **********/

/* allocate a new image of size width x height */
image_t_dm *dm_image_new(int width, int height)
{
  image_t_dm *image = NEW(image_t_dm);
  if(image == NULL)
    {
      err_printf( "Error: dm_image_new() - not enough memory !\n");
      exit(1);
    }
  image->width = width;
  image->height = height;
  image->stride = ( (width+3) / 4 ) * 4;
  image->data = NEWA(float, image->stride*height*sizeof(float));
  if(image->data == NULL)
    {
      err_printf( "Error: dm_image_new() - not enough memory !\n");
      exit(1);
    }
  return image;
}

/* allocate a new image and copy the content from src */
image_t_dm *dm_image_cpy(const image_t_dm *src)
{
  image_t_dm *dst = dm_image_new(src->width, src->height);
  memcpy(dst->data, src->data, src->stride*src->height*sizeof(float));
  return dst;
}

/* set all pixels values to zeros */
void dm_image_erase(image_t_dm *image)
{
  memset(image->data, 0, image->stride*image->height*sizeof(float));
}


/* multiply an image by a scalar */
void dm_image_mul_scalar(image_t_dm *image, float scalar)
{
  int i;
  for( i=0 ; i<image->stride*image->height ; i++)
    image->data[i] *= scalar;
}

/* free memory of an image */
void dm_image_delete(image_t_dm *image)
{
  if(image == NULL)
    {
      //err_printf( "Warning: Delete image --> Ignore action (image not allocated)\n");
    }
  else
    {
      free(image->data);
      free(image);
    }
}


/* allocate a new color image of size width x height */
color_image_t_dm *dm_color_image_new(int width, int height)
{
  size_t stride_channel = width*height*sizeof(float);
  char *buffer = NEWA(char, sizeof(color_image_t_dm) + 3*stride_channel);
  if(buffer == NULL)
    {
      err_printf( "Error: color_image_new() - not enough memory !\n");
      exit(1);
    }
  color_image_t_dm *image = (color_image_t_dm*) buffer;
  image->width = width;
  image->height = height;
  image->c1 = (float*) (buffer + sizeof(color_image_t_dm));
  image->c2 = (float*) (buffer + sizeof(color_image_t_dm) + stride_channel);
  image->c3 = (float*) (buffer + sizeof(color_image_t_dm) + 2*stride_channel);
  return image;
}

/* allocate a new color image and copy the content from src */
color_image_t_dm *dm_color_image_cpy(const color_image_t_dm *src)
{
  color_image_t_dm *dst = dm_color_image_new(src->width, src->height);
  memcpy(dst->c1, src->c1, 3*src->width*src->height*sizeof(float));
  return dst;
}

/* set all pixels values to zeros */
void dm_color_image_erase(color_image_t_dm *image)
{
    memset(image->c1, 0, 3*image->width*image->height*sizeof(float));
}

/* free memory of a color image */
void dm_color_image_delete(color_image_t_dm *image)
{
  if(image) 
    {
      free(image); // the image is allocated such that the data is stored just after the pointer
    }
}

/* convert a color image to a gray-scale image */
image_t_dm* dm_image_gray_from_color(color_image_t_dm *img)
{
  image_t_dm* res = dm_image_new(img->width, img->height);
  
  int n=0;
  for(int j=0; j<img->height; j++)
    for(int i=0; i<img->width; i++,n++)
    res->data[i+j*res->stride] = (img->c1[n] + img->c2[n] + img->c3[n])/3;
  
  return res;
}


/* reallocate the memory of an image to fit the new width height */
void dm_resize_if_needed_newsize(image_t_dm *im, int w, int h)
{
  if(im->width != w || im->height != h)
    {
      im->width = w;
      im->height = h;
      im->stride = ((w+3)/4)*4;
      float *data = NEWA(float,im->stride*h*sizeof(float));
      if(data == NULL)
        {
          err_printf( "Error: dm_resize_if_needed_newsize() - not enough memory !\n");
          exit(1);
        }
      free(im->data);
      im->data = data;
    }
}


/************ Resizing *********/

/* resize an image to a new size (assumes a difference only in width) */
void image_resize_horiz(image_t_dm *dst, const image_t_dm *src)
{
  int i;
  float real_scale = ((float) src->width-1) / ((float) dst->width-1);
  for(i = 0; i < dst->height; i++)
    {
      int j;
      for(j = 0; j < dst->width; j++)
        {
          float dx;
          int x;
          x = floor((float) j * real_scale);
          dx = j * real_scale - x; 
          if(x >= (src->width - 1))
            {
              dst->data[i * dst->stride + j] = 
                src->data[i * src->stride + src->width - 1]; 
            }
          else
            {
              dst->data[i * dst->stride + j] = 
                (1.0f - dx) * src->data[i * src->stride + x    ] + 
                (       dx) * src->data[i * src->stride + x + 1];
            }
        }
    }
}

/* resize an image to a new size (assumes a difference only in height) */
void image_resize_vert(image_t_dm *dst, const image_t_dm *src)
{
  int i;
  float real_scale = ((float) src->height-1) / ((float) dst->height-1);
  for(i = 0; i < dst->width; i++)
    {
      int j;
      for(j = 0; j < dst->height; j++)
        {
          int y;
          float dy;
          y = floor((float) j * real_scale);
          dy = j * real_scale - y;
          if(y >= (src->height - 1))
            {
              dst->data[j * dst->stride + i] =
                src->data[i + (src->height - 1) * src->stride]; 
            }
          else
            {
              dst->data[j * dst->stride + i] =
                (1.0f - dy) * src->data[i + (y    ) * src->stride] + 
                (       dy) * src->data[i + (y + 1) * src->stride];
            }
        }
    }
}

/* resize an image with bilinear interpolation to fit the new weidht, height ; reallocation is done if necessary */
void dm_image_resize_bilinear_newsize(image_t_dm *dst, const image_t_dm *src, int new_width, int new_height)
{
  dm_resize_if_needed_newsize(dst, new_width, new_height);
  if(new_width < new_height)
    {
      image_t_dm *tmp = dm_image_new(new_width, src->height);
      image_resize_horiz(tmp,src);
      image_resize_vert(dst,tmp);
      dm_image_delete(tmp);
    }
  else
    {
      image_t_dm *tmp = dm_image_new(src->width, new_height);
      image_resize_vert(tmp,src);
      image_resize_horiz(dst,tmp);
      dm_image_delete(tmp);
    }
}

/* resize an image with bilinear interpolation */
image_t_dm *dm_image_resize_bilinear_scale(const image_t_dm *src, float scale) {
  const int new_width = int(0.5 + src->width * scale);
  const int new_height = int(0.5 + src->height * scale);
  
  image_t_dm *res = dm_image_new(new_width, src->height);
  dm_image_resize_bilinear_newsize(res, src, new_width, new_height);
  return res;
}


/* crop an image (in-place) */
void dm_image_crop(image_t_dm *img, int width, int height)
{
  assert(width<=img->width);
  img->width = width;
  assert(height<=img->height);
  img->height = height;
}




























