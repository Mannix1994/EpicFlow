//
// Created by creator on 18-6-26.
//

#include "DeepMatching122/main.h"
#include "EpicFlow100/epicflow.h"
#include "SED/SED.h"

#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

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

int main(){
    const char *pre_name = "./image69.jpg";
    const char *next_name = "./image74.jpg";
    get_flow(pre_name,next_name);
    return 0;
}