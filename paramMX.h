#pragma once

#include "types.h"
#include "asserts.h"

real paramGeoMomentum(const MatrixGrayScale &image, real p, real q) {
	assertMatrixGrayScale(image);
	auto size = image.size();

	for (int x = 0; x < size.width; ++x) {
		for (int y = 0; y < size.height; ++y) {
		}
	}

	return 0;
}

real paramM7(const MatrixGrayScale &image){
	return ()/cv::pow(paramGeoMomentum(image, 0,0),4)
}