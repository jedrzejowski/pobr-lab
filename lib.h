#pragma once

#include <opencv2/core/hal/interface.h>

#define R 2
#define G 1
#define B 0

template<typename number_type>
uchar trim2uchar(number_type number) {
	if (number > 255) return 255;
	if (number < 0)return 0;
	return number;
}

