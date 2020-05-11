#pragma once

#include <opencv2/core/mat.hpp>

void assertMatrixRGB(const cv::Mat &matrix) {
	CV_Assert(matrix.type() == CV_8UC3);
}

void assertMatrixNormalizedRGB(const cv::Mat &matrix) {
	CV_Assert(matrix.type() == CV_32FC3);
}

void assertMatrixGrayScale(const cv::Mat &matrix) {
	CV_Assert(matrix.type() == CV_32FC1);
}


