//
// Created by creator on 18-6-26.
//

#include "DeepMatching122/main.h"
#include "EpicFlow100/epicflow.h"
#include "SED/SED.h"
#include "EpicFlow100/image.h"
#include "DeepMatching122/array_types.h"
#include "debug.h"

#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

/**
 * 三通道Mat转image_t_dm
 * @param image 三通道Mat
 * @return image_t_dm
 */
image_t_dm *Mat2Image(const Mat &image){
    if (image.type() != CV_8UC3){
        throw runtime_error("Not a CV_8UC3 mat");
    }
    image_t_dm *img = dm_image_new(image.cols,image.rows);
    for(int j=0; j<img->height; j++) {
        for (int i = 0; i < img->width; i++) {
            Vec3b pixel = image.at<Vec3b>(j,i);
            img->data[i+j*img->stride] = (pixel[0]+pixel[1]+pixel[2])/3;
        }
    }
    return img;
}

ef_color_image_t *Mat2ColorImage(const Mat &img){
    if (img.type() != CV_8UC3){
        throw runtime_error("Not a CV_8UC3 mat");
    }
    ef_color_image_t* image = ef_color_image_new(img.cols, img.rows);
    for(int i=0; i<img.rows; i++) {
        for(int j=0; j<img.cols; j++){
            Vec3b pixel = img.at<Vec3b>(i,j);
            image->c1[i*image->stride+j] = pixel[0];
            image->c2[i*image->stride+j] = pixel[1];
            image->c3[i*image->stride+j] = pixel[2];
        }
    }
    return image;
}

void get_flow(const char *pre_name,const char *next_name){

    //SED
    string modelFilename = "model.yml.gz";
    string outFilename = "edges.bin";
    Mat pre = imread(pre_name);
    if(pre.empty()){
        cerr<<__LINE__<<": pre is empty"<<endl;
        return;
    }
    sed(pre, modelFilename, outFilename);

    // DEEP_MATCHING
    const int argc = 7;
    const char *argv[argc] = {"null",pre_name,next_name,"-nt","0","-out","match.txt"};
    // 生成EpicFlow需要的match.txt
    main_deep_matching(argc,argv);

    // 调用epicflow
    const int argc_for_ef = 6;
    const char *argv_for_ef[argc_for_ef] = {"null", pre_name, next_name,outFilename.c_str(),
                                            "match.txt","out.flo"};
    main_epic_flow(argc_for_ef, (char**)argv_for_ef);
}

Mat get_flow_new(const char *pre_name,const char *next_name){

    //SED
    string modelFilename = "model.yml.gz";
    string outFilename = "edges.bin";
    Mat pre = imread(pre_name);
    Mat next = imread(next_name);
    if(pre.empty()){
        cerr<<__LINE__<<": pre is empty"<<endl;
        return Mat();
    }
    float_image edges = sed(pre, modelFilename);

    // DEEP_MATCHING
    const int argc = 7;
    const char *argv[argc] = {"null",pre_name,next_name,"-nt","0","-out","match.txt"};
    image_t_dm *pre_dm = Mat2Image(pre);
    image_t_dm *next_dm = Mat2Image(next);
    //
    float_image matches = main_deep_matching(argc,argv,pre_dm,next_dm);

    // 调用epicflow
    const int argc_for_ef = 6;
    const char *argv_for_ef[argc_for_ef] = {"null", pre_name, next_name,outFilename.c_str(),
                                            "match.txt","new.flo"};
    ef_color_image_t *pre_ef = Mat2ColorImage(pre);
    ef_color_image_t *next_ef = Mat2ColorImage(next);
    return main_epic_flow(argc_for_ef, (char**)argv_for_ef,pre_ef,next_ef,edges,matches);
}

int main(){
    const char *pre_name = "./image69.jpg";
    const char *next_name = "./image74.jpg";
    get_flow(pre_name,next_name);
    get_flow_new(pre_name,next_name);
    return 0;
}