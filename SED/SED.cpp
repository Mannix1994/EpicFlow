//
// Created by creator on 18-6-26.
//

#include <opencv2/ximgproc.hpp>
#include "SED.h"


using namespace std;
using namespace cv;
using namespace cv::ximgproc;

float_image sed(const Mat &image, const std::string &modelFilename){
    Mat img;
    image.convertTo(img, DataType<float>::type, 1 / 255.0);

    Mat edges(image.size(), image.type());

    Ptr<StructuredEdgeDetection> pDollar =
            createStructuredEdgeDetection(modelFilename);

    pDollar->detectEdges(img, edges);

    float_image f_image= empty_image(float, edges.cols, edges.rows);

    for (int i = 0; i < edges.rows; i++){
        for (int j = 0; j < edges.cols; j++){
            int index = j + i * edges.cols;
            f_image.pixels[index] = edges.at<float>(i,j);
        }
    }
    return f_image;
}

void sed(Mat image, string modelFilename, string outFilename) {
    Mat img;
    image.convertTo(img, DataType<float>::type, 1 / 255.0);

    Mat edges(image.size(), image.type());

    Ptr<StructuredEdgeDetection> pDollar =
            createStructuredEdgeDetection(modelFilename);
    pDollar->detectEdges(img, edges);
    //imshow("sed",edges);waitKey(0);
    Mat d = edges.clone();
    imwrite("edges.bmp", 255 * d);

    ofstream fp(outFilename, ios::binary);
    float p[edges.cols][edges.rows];   //存转置
    for (int i = 0; i < edges.rows; i++)
        for (int j = 0; j < edges.cols; j++)
            p[j][i] = edges.at<float>(i, j);
    fp.write((char *) p, edges.cols * edges.rows * sizeof(float));
    fp.close();
}
