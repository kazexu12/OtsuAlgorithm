#include<opencv2/opencv.hpp>
int main()
{

	cv::Mat image1, image2;

	image1 = cv::imread("./bot.jpg");
	cv::namedWindow("Car", cv::WINDOW_AUTOSIZE);

	cv::imshow("Car", image1);
	//Car window is used 
	cv::imshow(" image1", image1);
	// created the window by name image1
	cv::waitKey(0);
	cv::destroyWindow("Car");

	return 0;
}