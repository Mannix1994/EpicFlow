//
// Created by creator on 18-6-26.
//

#ifndef EPICFLOW_SED_H
#define EPICFLOW_SED_H

#include "../EpicFlow100/array_types.h"

#include <string>
#include <opencv2/opencv.hpp>

float_image sed(const cv::Mat &image, const std::string &modelFilename);
void sed(cv::Mat image,std::string modelFilename,std::string outFilename);

#endif //EPICFLOW_SED_H
