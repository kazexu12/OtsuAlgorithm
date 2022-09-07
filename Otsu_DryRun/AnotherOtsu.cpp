#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <string>
#include <stdio.h>

using namespace std;
using namespace cv;
#define Gamma 3
//OTSU function implementation 
int OTSU(Mat srcImage) {
    int nCols = srcImage.cols;
    int nRows = srcImage.rows;
    int threshold = 0;
    //init the parameters
    int nSumPix[256];
    float nProDis[256];
    for (int i = 0; i < 256; i++)
    {
        nSumPix[i] = 0;
        nProDis[i] = 0;
    }

    //Count the number of each pixel in the entire image in the grayscale set 
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            nSumPix[(int)srcImage.at<uchar>(i, j)]++;
        }
    }

    //Calculate the probability distribution of each gray level in the image 
    for (int i = 0; i < 256; i++)
    {
        nProDis[i] = (float)nSumPix[i] / (nCols * nRows);
    }

    //Traverse the gray level [0, 255], calculate the threshold under the maximum variance between classes

    float w0, w1, u0_temp, u1_temp, u0, u1, delta_temp;
    double delta_max = 0.0;
    for (int i = 0; i < 256; i++)
    {
        //Initialize related parameters
        w0 = w1 = u0 = u1 = u0_temp = u1_temp = delta_temp = 0;
        for (int j = 0; j < 256; j++)
        {
            //background part 
            if (j <= i)
            {
                w0 += nProDis[j];
                u0_temp += j * nProDis[j];
            }
            //Foreground part 
            else
            {
                w1 += nProDis[j];
                u1_temp += j * nProDis[j];
            }
        }
        //Calculate the average gray level of the two categories
        u0 = u0_temp / w0;
        u1 = u1_temp / w1;
        //Find the threshold value under the largest inter-class variance in turn
        delta_temp = (float)(w0 * w1 * pow((u0 - u1), 2)); //Variance between foreground and background (inter-class variance)
        if (delta_temp > delta_max)
        {
            delta_max = delta_temp;
            threshold = i;
        }
    }
    return threshold;
}


int  main()
{

    namedWindow("srcGray", 0);
    cvResizeWindow("srcGray", 640, 480);
    namedWindow("otsuResultImage", 0);
    cvResizeWindow("otsuResultImage", 640, 480);
    namedWindow("dst", 0);
    cvResizeWindow("dst", 640, 480);
    //Image reading and judgment
    Mat srcImage;
    srcImage = imread("image\\001.jpg");
    if (!srcImage.data)
    {
        return -1;
    }
    Mat srcGray;
    cvtColor(srcImage, srcGray, CV_RGB2GRAY);
    imshow("srcGray", srcGray);

    //Call the otsu algorithm to get the image 
    int otsuThreshold = OTSU(srcGray);
    cout << otsuThreshold << endl;
    //Define the output result image
    Mat otsuResultImage = Mat::zeros(srcGray.rows, srcGray.cols, CV_8UC1);

    //Use the obtained threshold to perform binarization 
    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            //cout << (int)srcGray.at<uchar>(i, j) << endl;
            //High pixel threshold judgment 
            if (srcGray.at<uchar>(i, j) > otsuThreshold)
            {
                otsuResultImage.at<uchar>(i, j) = 255;
            }
            else
            {
                otsuResultImage.at<uchar>(i, j) = 0;
            }
            //cout <<(int)otsuResultImage.at<uchar>(i, j) << endl;
        }
    }
    imshow("otsuResultImage", otsuResultImage);
    waitKey(0);
    return 0;
}