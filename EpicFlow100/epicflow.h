//
// Created by creator on 18-6-26.
//

#ifndef EDGEDETECTION_EPICFLOW_H
#define EDGEDETECTION_EPICFLOW_H

#include "image.h"

int main_epic_flow(int argc, char **argv);
int main_epic_flow(int argc, char **argv,
                   ef_color_image_t *im1,
                   ef_color_image_t *im2,
                   float_image edges,
                   float_image matches);
#endif //EDGEDETECTION_EPICFLOW_H
