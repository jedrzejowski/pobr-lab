#pragma once

#include <opencv2/core/mat.hpp>

void assertMatrixRGB(const cv::Mat &matrix) {
	static auto source = MatrixRGB();
	CV_Assert(matrix.type() == source.type());
}

void assertMatrixNormalizedRGB(const cv::Mat &matrix) {
	static auto source = MatrixNormalizedRGB();
	CV_Assert(matrix.type() == source.type());
}

void assertMatrixGrayScale(const cv::Mat &matrix) {
	static auto source = MatrixGrayScale();
	CV_Assert(matrix.type() == source.type());
}


