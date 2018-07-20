//
// Created by creator on 18-7-3.
//
#include "DeepMatching122/main.h"
#include "EpicFlow100/epicflow.h"
#include "SED/SED.h"
#include "EpicFlow100/image.h"
#include "DeepMatching122/array_types.h"
#include "debug.h"
#include "EpicFlowWithDeepMatching.h"

using namespace cv;
using namespace std;

/**
 * 三通道Mat转image_t_dm
 * @param image 三通道Mat
 * @return image_t_dm
 */
image_t_dm *mat2Gray(const Mat &image){
    image_t_dm *img = dm_image_new(image.cols,image.rows);
    if(image.type() == CV_8UC3) {
        for (int j = 0; j < img->height; j++) {
            for (int i = 0; i < img->width; i++) {
                Vec3b pixel = image.at<Vec3b>(j, i);
                img->data[i + j * img->stride] = (pixel[0] + pixel[1] + pixel[2]) / 3.0f;
            }
        }
    } else if(image.type()==CV_8UC1){
        for (int j = 0; j < img->height; j++) {
            for (int i = 0; i < img->width; i++) {
                uchar pixel = image.at<uchar>(j, i);
                img->data[i + j * img->stride] = pixel;
            }
        }
    } else{
        throw runtime_error("Not a CV_8UC3 or CV_8UC1 mat");
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

/**
 * 老版本，通过文件来传参数的函数，会保存一个.flo光流文件
 * @param pre_name the pre image file path(support .jpg .png image file)
 * @param next_name the next image file path
 */
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

/**
 * 新版本，不通过文件传参的光流函数
 * new version flow function
 * @param pre the pre image(Mat)
 * @param next the next image(Mat)
 * @return a Mat of flow,the first channel is optic flow at x direction,
 * and the second channel is optic flow in y direction.
 */
Mat get_flow(const Mat &pre, const Mat &next){
    // get edges
    const string modelFilename = "model.yml.gz";
    float_image edges = sed(pre, modelFilename);

    // DEEP_MATCHING
    const int argc = 7;
    // 下面这排参数几乎没用，只是为了保证在最少更改原程序的条件下保证程序正常运行
    const char *argv[argc] = {"null","pre_name","next_name","-nt","0","-out","match.txt"};
    image_t_dm *pre_gray = mat2Gray(pre);
    image_t_dm *next_gray = mat2Gray(next);
    // 得到matches
    float_image matches = main_deep_matching(argc,argv,pre_gray,next_gray);

    // 调用epicflow，计算光流
    const int argc_for_ef = 6;
    const char *argv_for_ef[argc_for_ef] = {"null", "pre_name", "next_name","edges.bin",
                                            "match.txt","new.flo"};
    ef_color_image_t *pre_ef = Mat2ColorImage(pre);
    ef_color_image_t *next_ef = Mat2ColorImage(next);
    return main_epic_flow(argc_for_ef, (char**)argv_for_ef,pre_ef,next_ef,edges,matches);
}

/**
 * 新版本，不通过文件传参的光流函数
 * new version flow function
 * @param pre the pre image file path(support .jpg .png image file)
 * @param next the next image file path
 * @return a Mat of flow,the first channel is optic flow at x direction,
 * and the second channel is optic flow in y direction.
 */
Mat get_flow_new(const char *pre_name,const char *next_name){

    Mat pre = imread(pre_name);
    Mat next = imread(next_name);
    if(pre.empty()){
        cerr<<__LINE__<<": pre is empty"<<endl;
        return Mat();
    }if(next.empty()){
        cerr<<__LINE__<<": next is empty"<<endl;
        return Mat();
    }
    CV_Assert(pre.type() == next.type());
    return get_flow(pre,next);
}