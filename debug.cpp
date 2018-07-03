//
// Created by creator on 18-7-3.
//

#include "debug.h"
#include <fstream>
#include <iostream>
using namespace std;

void save_float_image_to_csv(const float_image &image, const char* name){
    if(name== nullptr){
        return;
    }
    ofstream o(name);
    if(!o.is_open()){
        cerr<<"打开文件"+string(name)+"失败"<<endl;
        return;
    }
    const int tx = image.tx;
    const int ty = image.ty;
    for(int i=0;i<image.ty;i++){
        for(int j=0;j<image.tx;j++){
            o<<image.pixels[j+i*tx]<<",";
        }
        o<<endl;
    }
    o.close();
}

void save_flow_to_csv(const char *filename, const ef_image_t *flowx, const ef_image_t *flowy) {
    ofstream stream(filename);
    if(!stream.is_open()){
        fprintf(stderr, "Error while opening %s\n",filename);
        exit(-1);
    }
    const float help=202021.25f;
    const int aXSize = flowx->width, aYSize = flowx->height;
    stream<< help <<","<< aXSize<<","<<aYSize<<endl;
    int y,x;
    for (y = 0; y < aYSize ; y++) {
        for (x = 0; x < aXSize; x++) {
            stream<<flowx->data[y * flowx->stride + x]<<","<<flowy->data[y * flowy->stride + x]<<",";
        }
        stream<<endl;
    }
    stream.close();
}

void save_flow_to_csv(const char *filename, const cv::Mat &flow) {
    ofstream stream(filename);
    if(!stream.is_open()){
        fprintf(stderr, "Error while opening %s\n",filename);
        exit(-1);
    }
    const float help=202021.25f;
    const int aXSize = flow.cols, aYSize = flow.rows;
    stream<< help <<","<< aXSize<<","<<aYSize<<endl;
    int y,x;
    for (y = 0; y < aYSize ; y++) {
        for (x = 0; x < aXSize; x++) {
            cv::Vec2f pixel = flow.at<cv::Vec2f>(y,x);
            stream<<pixel[0]<<","<<pixel[1]<<",";
        }
        stream<<endl;
    }
    stream.close();
}
