#pragma once

#include <opencv2/core/mat.hpp>

using WekselRGB = cv::Vec3b;
using MatrixRGB = cv::Mat_<WekselRGB>;

using WekselNormalizedRGB = cv::Vec3f;
using MatrixNormalizedRGB = cv::Mat_<WekselNormalizedRGB>;

using WekselGrayScale = cv::Vec<float, 1>;
using MatrixGrayScale = cv::Mat_<WekselGrayScale>;