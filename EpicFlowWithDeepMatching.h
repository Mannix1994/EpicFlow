//
// Created by creator on 18-7-3.
//

#ifndef EPICFLOW_EPICFLOWWITHDEEPMATCHING_H
#define EPICFLOW_EPICFLOWWITHDEEPMATCHING_H

#include <opencv2/opencv.hpp>

void get_flow(const char *pre_name,const char *next_name);
cv::Mat get_flow_new(const char *pre_name,const char *next_name);
cv::Mat get_flow(const cv::Mat &pre, const cv::Mat &next);

#endif //EPICFLOW_EPICFLOWWITHDEEPMATCHING_H
