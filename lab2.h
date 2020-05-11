#pragma once

#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "lib.h"

template<typename MatrixType, typename FilterType>
cv::Mat_<MatrixType> my_filter2D(
		const cv::Mat_<MatrixType> &image,
		const cv::Mat_<FilterType> &matrix
) {

	CV_Assert(matrix.rows % 2 == 1);
	CV_Assert(matrix.cols % 2 == 1);

	auto size = image.size();

	cv::Mat_<MatrixType> output = cv::Mat(size, image.type());

	auto get_pixel = [&](int x, int y) -> MatrixType {
		if (x < 0) x = 0;
		if (x >= size.width) x = size.width - 1;

		if (y < 0) y = 0;
		if (y >= size.height) y = size.height - 1;

		return image.template at<MatrixType>(cv::Point(x, y));
	};

	int dx = (matrix.cols - 1) / 2;
	int dy = (matrix.rows - 1) / 2;
	auto multiply = [&](const int &cx, const int &cy) {

		MatrixType sum_pixel;

		for (int mx = 0; mx < matrix.cols; ++mx) {
			for (int my = 0; my < matrix.rows; ++my) {

				auto pixel = get_pixel(cx - dx + mx, cy - dy + my);
				auto factor = matrix(cv::Point(mx, my));

				sum_pixel += (pixel * factor);
			}
		}


		output(cv::Point(cx, cy)) = sum_pixel;
	};

	for (int x = 0; x < image.cols; ++x) {
		for (int y = 0; y < image.rows; ++y) {
			multiply(x, y);
		}
	}

//	cv::threshold(output, output, 0, 1, 3);

	return output;
}


const std::string lena_path = "../Lena.bmp";

int main_lab2(int argl, char *argv[]) {
//	std::cout << "Start ..." << std::endl;
//
//	cv::Mat image = normalizeMatOfVec3bToMatOfVec3f(cv::imread(lena_path));
//
//	if (image.empty()) {
//		std::cerr << "Error during reading " << lena_path << std::endl;
//		return 1;
//	}
//
//	// https://docs.opencv.org/3.4/d6/d6d/tutorial_mat_the_basic_image_container.html
//	cv::Mat matrix1 = (cv::Mat_<float>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
//	cv::Mat matrix2 = (cv::Mat_<float>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);
//	cv::Mat matrix3 = matrix1 / 9;
//
//	std::cout << "matrix1" << matrix1 << std::endl;
//	std::cout << "matrix2" << matrix2 << std::endl;
//
//	auto output1 = my_filter2D(image, matrix1);
//	auto output2 = my_filter2D(image, matrix2);
//	auto output3 = my_filter2D(image, matrix3);
//
//	cv::imshow("Lena.png", image);
//	cv::imshow("matrix1", output1);
//	cv::imshow("matrix2", output2);
//	cv::imshow("matrix3", output3);
//
//	cv::imwrite("../matrix1.png", output1);
//	cv::imwrite("../matrix2.png", output2);
//	cv::imwrite("../matrix3.png", output3);
//
//	cv::waitKey(-1);
}