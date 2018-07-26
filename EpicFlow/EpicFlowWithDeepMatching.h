//
// Created by creator on 18-7-3.
//

#ifndef EPICFLOW_EPICFLOWWITHDEEPMATCHING_H
#define EPICFLOW_EPICFLOWWITHDEEPMATCHING_H

#include <opencv2/opencv.hpp>

//void get_flow(const char *pre_name,const char *next_name);

/**
 * 新版本，不通过文件传参的光流函数
 * new version flow function
 * @param pre the pre image file path(support .jpg .png image file)
 * @param next the next image file path
 * @return a Mat of flow,the first channel is optic flow at x direction,
 * and the second channel is optic flow in y direction.
 */
cv::Mat get_flow_new(const char *pre_name,const char *next_name);
/**
 * 新版本，不通过文件传参的光流函数
 * new version flow function
 * @param pre the pre image(Mat)
 * @param next the next image(Mat)
 * @return a Mat of flow,the first channel is optic flow at x direction,
 * and the second channel is optic flow in y direction.
 */
cv::Mat get_flow(const cv::Mat &pre, const cv::Mat &next);

#endif //EPICFLOW_EPICFLOWWITHDEEPMATCHING_H
