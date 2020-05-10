#pragma once

#include <opencv2/core/hal/interface.h>

#define R 2
#define G 1
#define B 0

using NormalizedPixel = cv::Vec3f;
using NormalizedImage = cv::Mat_<NormalizedPixel>;

template<typename number_type>
uchar trim2uchar(number_type number) {
	if (number > 255) return 255;
	if (number < 0)return 0;
	return number;
}

float trimFloat(float number) {
	if (number > 1) return 1;
	if (number < 0)return 0;
	return number;
}

NormalizedImage normalizeMatOfVec3bToMatOfVec3f(const cv::Mat_<cv::Vec3b> &matrix) {

	auto output = cv::Mat_<cv::Vec3f>(matrix.rows, matrix.cols);

	matrix.forEach([&](cv::Vec3b &pixel, const int position[]) -> void {
		output(position[0], position[1]) = pixel / 255;
	});

	return output;
}

cv::Mat_<cv::Vec3b> denormalizeMatOfVec3bToMatOfVec3f(const NormalizedImage &matrix) {

	auto output = cv::Mat_<cv::Vec3b>(matrix.rows, matrix.cols);

	for (int x = 0; x < matrix.cols; ++x) {
		for (int y = 0; y < matrix.rows; ++y) {
			output(x, y) = matrix(x, y) * 225;
		}
	}

	return output;
}

bool areMatrixEqual(const NormalizedImage &img1, const NormalizedImage &img2) {
	return (sum(img1 != img2) == cv::Scalar(0, 0, 0, 0));
}

bool
isColorWithInTolerance(const NormalizedPixel &first, const NormalizedPixel &second, const NormalizedPixel &tolerance) {
	return std::abs(second[0] - first[0]) <= std::abs(tolerance[0]) &&
		   std::abs(second[1] - first[1]) <= std::abs(tolerance[1]) &&
		   std::abs(second[2] - first[2]) <= std::abs(tolerance[2]);
}
