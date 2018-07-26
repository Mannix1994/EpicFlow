//
// Created by creator on 18-7-3.
//

#ifndef EPICFLOW_DEBUG_H
#define EPICFLOW_DEBUG_H


#include <opencv2/core/mat.hpp>
#include "EpicFlow100/array_types.h"
#include "EpicFlow100/image.h"

void save_float_image_to_csv(const float_image &image, const char* name);
void save_flow_to_csv(const char *filename, const ef_image_t *flowx, const ef_image_t *flowy);
void save_flow_to_csv(const char *filename, const cv::Mat &flow);

#endif //EPICFLOW_DEBUG_H
