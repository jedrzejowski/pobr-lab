#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "lab2.h"
#include <iostream>
#include <string>

const std::string lena_path = "../Lena.bmp";

int main(int, char *[]) {
	std::cout << "Start ..." << std::endl;

	cv::Mat image = cv::imread(lena_path);

	if (image.empty()) {
		std::cerr << "Error during reading " << lena_path << std::endl;
		return 1;
	}

	// https://docs.opencv.org/3.4/d6/d6d/tutorial_mat_the_basic_image_container.html
	cv::Mat matrix1 = (cv::Mat_<double>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	cv::Mat matrix2 = (cv::Mat_<double>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);
	cv::Mat matrix3 = matrix1 / 9;

	std::cout << "matrix1" << matrix1 << std::endl;
	std::cout << "matrix2" << matrix2 << std::endl;

	auto output1 = lab2(image, matrix1);
	auto output2 = lab2(image, matrix2);
	auto output3 = lab2(image, matrix3);

	cv::imshow("Lena.png", image);
	cv::imshow("matrix1", output1);
	cv::imshow("matrix2", output2);
	cv::imshow("matrix3", output3);

	cv::imwrite("../matrix1.png", matrix1);
	cv::imwrite("../matrix2.png", matrix2);
	cv::imwrite("../matrix3.png", matrix3);

	cv::waitKey(-1);
	return 0;
}