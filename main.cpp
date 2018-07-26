//
// Created by creator on 18-6-26.
//

#include <opencv2/opencv.hpp>
#include "EpicFlowWithDeepMatching.h"

using namespace std;
using namespace cv;

int main(){
    const char *pre_name = "./climb1.png";
    const char *next_name = "./climb2.png";
//    get_flow(pre_name,next_name);
    Mat pre = imread(pre_name);
    Mat next = imread(next_name);
    for(int i=0;i<1;i++)
        Mat flow = get_flow(pre,next);
//    save_flow_to_csv("mat_flo.csv",flow);
    return 0;
}