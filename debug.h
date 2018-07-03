//
// Created by creator on 18-7-3.
//

#ifndef EPICFLOW_DEBUG_H
#define EPICFLOW_DEBUG_H


#include "EpicFlow100/array_types.h"
#include "EpicFlow100/image.h"

void save_float_image_to_csv(const float_image &image, const char* name);
void save_flow_to_csv(const char *filename, const ef_image_t *flowx, const ef_image_t *flowy);

#endif //EPICFLOW_DEBUG_H
