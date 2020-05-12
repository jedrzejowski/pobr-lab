#pragma once

#include "types.h"

namespace param {


	real W3(const real &area, const real &perimeter) {
		return (perimeter / (2 * cv::sqrt(area * CV_PI))) - 1;
	}
}

