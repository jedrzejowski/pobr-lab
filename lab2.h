#pragma once

#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include "lib.h"

cv::Mat lab2(cv::Mat &image, cv::Mat &matrix) {
	CV_Assert(image.depth() != sizeof(uchar));
	CV_Assert(matrix.depth() != sizeof(double));
	CV_Assert(image.channels() == 3);
	CV_Assert(matrix.rows % 2 == 1);
	CV_Assert(matrix.cols % 2 == 1);

	cv::Mat output(image.rows, image.cols, CV_8UC3);

	cv::Mat_<cv::Vec3b> _image = image;
	cv::Mat_<cv::Vec3b> _output = output;

	auto get_pixel = [&](int x, int y) -> cv::Vec3b {
		if (x < 0) x = 0;
		if (x >= image.cols) x = image.cols - 1;

		if (y < 0) y = 0;
		if (y >= image.rows) y = image.rows - 1;

		return _image(x, y);
	};

	int dx = (matrix.cols - 1) / 2;
	int dy = (matrix.rows - 1) / 2;
	auto multiply = [&](const int &cx, const int &cy) {

		cv::Vec3d sum_pixel;

		for (int mx = 0; mx < matrix.cols; ++mx) {
			for (int my = 0; my < matrix.rows; ++my) {

				auto pixel = get_pixel(cx - dx + mx, cy - dy + my);
				auto factor = matrix.at<double>(mx, my);

				sum_pixel[R] += pixel[R] * factor;
				sum_pixel[G] += pixel[G] * factor;
				sum_pixel[B] += pixel[B] * factor;
			}
		}

		cv::Vec3b out_pixel = 0;
		out_pixel[R] = trim2uchar(sum_pixel[R]);
		out_pixel[G] = trim2uchar(sum_pixel[G]);
		out_pixel[B] = trim2uchar(sum_pixel[B]);

		_output(cx, cy) = out_pixel;
	};

	for (int x = 0; x < image.cols; ++x) {
		for (int y = 0; y < image.rows; ++y) {
			multiply(x, y);
		}
	}

	output = _output;
	return output;
}