// Otsu_DryRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
    //read the image in BGR format 
    //BGR stands for Blue(255, 0, 0), Green(0, 255, 0), Red(0, 0, 255). 
    //OpenCV uses BGR color as a default color space to display images, when we open a image in openCV using cv2. 
    //imread() it display the image in BGR format. And it provides color-changing methods using cv2.

    Mat testImage = imread("bot.jpg", 0);
    //The Mat class of OpenCV library is used to store the values of an image. 

    int bins_num = 256;

    // Get the histogram
    long double histogram[256];

    //initialize all intensity values to 0
    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    //calculate the no of pixels for each intensity values
    for (int y = 0; y < testImage.rows; y++) {
        for (int x = 0; x < testImage.cols; x++) {
            histogram[(int)testImage.at<uchar>(x, y)]++;
            //at is an overloaded C++ template function of the class cv::Mat.
            //The < > is the syntax for invoking a C++ template.
            //The above line means, we are accessing the pixel (x,y) and specifying its data type to be unsigned char.
        }
    }

    // Calculate the bin_edges
    long double bin_edges[256];
    bin_edges[0] = 0.0;
    long double increment = 0.99609375;
    for (int i = 1; i < 256; i++)
        bin_edges[i] = bin_edges[i - 1] + increment;

    // Calculate bin_mids
    long double bin_mids[256];
    for (int i = 0; i < 256; i++) {
        bin_mids[i] = (bin_edges[i] + bin_edges[i + 1]) / 2;
    }

    // Iterate over all thresholds (indices) and get the probabilities weight1, weight2
    long double weight1[256];
    weight1[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        weight1[i] = histogram[i] + weight1[i - 1];
    }

    int total_sum = 0;
    for (int i = 0; i < 256; i++) {
        total_sum = total_sum + histogram[i];
    }

    long double weight2[256];
    weight2[0] = total_sum;
    for (int i = 1; i < 256; i++) {
        weight2[i] = weight2[i - 1] - histogram[i] * bin_mids[i];
    }

    // Calculate the class means: mean1 and mean2
    long double histogram_bin_mids[256];
    for (int i = 0; i < 256; i++) {
        histogram_bin_mids[i] = histogram[i] * bin_mids[i];
    }

    long double cumsum_mean1[256];
    cumsum_mean1[0] = histogram_bin_mids[0];
    for (int i = 1; i < 256; i++) {
        cumsum_mean1[i] = cumsum_mean1[i - 1] + histogram_bin_mids[i];
    }

    long double cumsum_mean2[256];
    cumsum_mean2[0] = histogram_bin_mids[255];
    for (int i = 1, j = 254; i < 256 && j >= 0; i++, j--) {
        cumsum_mean2[i] = cumsum_mean2[i - 1] + histogram_bin_mids[j];
    }

    long double mean1[256];
    for (int i = 0; i < 256; i++) {
        mean1[i] = cumsum_mean1[i] / weight1[i];
    }

    long double mean2[256];
    for (int i = 0, j = 255; i < 256 && j >= 0; i++, j--) {
        mean2[j] = cumsum_mean2[i] / weight2[j];
    }

    // Calculate Inter_class_variance
    long double Inter_class_variance[255];
    long double dnum = 10000000000;
    for (int i = 0; i < 255; i++) {
        Inter_class_variance[i] = ((weight1[i] * weight2[i] * (mean1[i] - mean2[i + 1])) / dnum) * (mean1[i] - mean2[i + 1]);
    }

    // Maximize interclass variance
    long double maxi = 0;
    int getmax = 0;
    for (int i = 0; i < 255; i++) {
        if (maxi < Inter_class_variance[i]) {
            maxi = Inter_class_variance[i];
            getmax = i;
        }
    }
    cout << "\n";
    cout << "\n";
    cout << "Otsu's algorithm implementation thresholding result:" << bin_mids[getmax];

    Mat dst;
    double thresh = 0;
    double maxValue = 255;
    cout << "\n";
    cout << "\n";
    long double thres = cv::threshold(testImage, dst, thresh, maxValue, THRESH_OTSU);
    cout << "Otsu Threshold : " << thres << endl;
}

