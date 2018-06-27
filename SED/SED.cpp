//
// Created by creator on 18-6-26.
//

#include "SED.h"
#include <opencv2/ximgproc.hpp>

using namespace std;
using namespace cv;
using namespace cv::ximgproc;

void sed(Mat image, string modelFilename, string outFilename) {
    Mat img;
    image.convertTo(img, DataType<float>::type, 1 / 255.0);

    Mat edges(image.size(), image.type());

    Ptr<StructuredEdgeDetection> pDollar =
            createStructuredEdgeDetection(modelFilename);
    pDollar->detectEdges(img, edges);
    //imshow("sed",edges);waitKey(0);
    Mat d = edges.clone();
//    for(int i=0;i<d.rows;i++)
//    {
//        for(int j=0;j<d.cols;j++)
//        {
//            //if(255*d.at<float>(i,j)<=5)
//               // d.at<float>(i,j)=0;
//           // else d.at<float>(i,j)=1;
//        }
//    }
    imwrite("edges.bmp", 255 * d);

    ofstream fp(outFilename, ios::binary);
    float p[edges.cols][edges.rows];   //存转置
    for (int i = 0; i < edges.rows; i++)
        for (int j = 0; j < edges.cols; j++)
            p[j][i] = edges.at<float>(i, j);
    fp.write((char *) p, edges.cols * edges.rows * sizeof(float));
    fp.close();
}
