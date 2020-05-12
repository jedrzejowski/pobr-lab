#pragma once

#include <opencv2/core/hal/interface.h>
#include "types.h"
#include "asserts.h"

#define R 2
#define G 1
#define B 0


template<typename number_type>
uchar trim2uchar(number_type number) {
	if (number > 255) return 255;
	if (number < 0)return 0;
	return number;
}

real trimFloat(real number) {
	if (number > 1) return 1;
	if (number < 0)return 0;
	return number;
}

bool isPointInSize(const cv::Point &point, const cv::Size &size) {
	return point.x >= 0 && point.y >= 0 &&
		   point.x < size.width && point.y < size.height;
}

void clampGrayScaleMatrix(const MatrixGrayScale &matrix) {
	matrix.forEach([&](WekselGrayScale &pixel, const int position[]) -> void {
		pixel[0] = std::clamp(pixel[0], 0.f, 1.f);
	});
}

//https://stackoverflow.com/questions/15888180/calculating-the-angle-between-points
float angleBetweenVectors(const cv::Point &v1, const cv::Point &v2) {
	float len1 = cv::sqrt(v1.x * v1.x + v1.y * v1.y);
	float len2 = cv::sqrt(v2.x * v2.x + v2.y * v2.y);

	float dot = v1.x * v2.x + v1.y * v2.y;

	float a = dot / (len1 * len2);


	return acos(a); // 0..PI
}

MatrixNormalizedRGB normalizeMatOfVec3bToMatOfVec3f(const MatrixRGB &matrix) {
	assertMatrixRGB(matrix);

	auto output = cv::Mat_<cv::Vec3f>(matrix.rows, matrix.cols);

	matrix.forEach([&](const WekselRGB &pixel, const int position[]) -> void {
		output(position[0], position[1]) = WekselNormalizedRGB(pixel) / 255;
	});

	return output;
}

MatrixRGB denormalizeMatOfVec3bToMatOfVec3f(const MatrixNormalizedRGB &matrix) {
	assertMatrixNormalizedRGB(matrix);

	auto output = MatrixRGB(matrix.rows, matrix.cols);

	matrix.forEach([&](const WekselNormalizedRGB &pixel, const int position[]) -> void {
		output(position[0], position[1]) = WekselRGB(pixel * 255);
	});

	return output;
}

bool areMatrixEqual(const MatrixNormalizedRGB &img1, const MatrixNormalizedRGB &img2) {
	return (sum(img1 != img2) == cv::Scalar(0, 0, 0, 0));
}

bool
isColorWithInTolerance(const WekselNormalizedRGB &first, const WekselNormalizedRGB &second,
					   const WekselNormalizedRGB &tolerance) {
	return std::abs(second[0] - first[0]) <= std::abs(tolerance[0]) &&
		   std::abs(second[1] - first[1]) <= std::abs(tolerance[1]) &&
		   std::abs(second[2] - first[2]) <= std::abs(tolerance[2]);
}
