#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "image.h"
#include "array_types.h"

/* read edges from a binary file containing width*height float32 values */
float_image ef_read_edges(const char *filename, const int width, const int height);

/* read matches, stored as x1 y1 x2 y2 per line (other values on the same is not taking into account */
float_image ef_read_matches(const char *filename);

/* read a flow file and returns a pointer with two images containing the flow along x and y axis */
ef_image_t** ef_readFlowFile(const char *filename);

/* write a flow to a file */
void ef_writeFlowFile(const char *filename, const ef_image_t *flowx, const ef_image_t *flowy);

/* load a color image from a file in jpg or ppm*/
ef_color_image_t *ef_color_image_load(const char *fname);


#ifdef __cplusplus
}
#endif
