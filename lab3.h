#include <queue>
#include <list>
#include <numeric>
#include <atomic>
#include "types.h"
#include "asserts.h"
#include "paramWX.h"
#include "paramMX.h"


static const cv::Mat edge_filter_matrix = (
		cv::Mat_<real>(3, 3)
				<<
				0, -1, 0,
				-1, 4, -1,
				0, -1, 0
);

MatrixNormalizedRGB edgeFilter(const MatrixNormalizedRGB &image) {
	assertMatrixNormalizedRGB(image);

	return my_filter2D<WekselNormalizedRGB, float>(image, edge_filter_matrix);
}

MatrixGrayScale findFigure(
		const MatrixNormalizedRGB &image,
		const cv::Point2i &seed,
		const WekselNormalizedRGB &tolerance = WekselNormalizedRGB(0, 0, 0)
) {

	const auto size = image.size();
	cv::Mat done = cv::Mat::zeros(size, CV_8U);
	MatrixGrayScale output = MatrixGrayScale::zeros(size);

	struct Step {
		cv::Point point;
		WekselNormalizedRGB color;
	};

	auto queue = std::queue<Step>();
	auto point = seed;

	auto push = [&](const cv::Point &point, const WekselNormalizedRGB &pixel) -> void {
		if (!isPointInSize(point, size)) {
			return;
		}

		if (done.at<char>(point) == 1) {
			return;
		}

		done.at<char>(point) = 1;
		queue.push(Step{point, pixel});
	};

	push(point, image(point));

	// tu nie może byc for_each
	do {
		auto step = queue.front();

		auto old_pixel = step.color;
		point = step.point;

		auto new_pixel = image(point);

		if (old_pixel == new_pixel) {
			output.at<WekselGrayScale>(point) = 1;

			push(point + cv::Point(+1, 0), new_pixel);
			push(point + cv::Point(-1, 0), new_pixel);
			push(point + cv::Point(0, +1), new_pixel);
			push(point + cv::Point(0, -1), new_pixel);
		}

		queue.pop();
	} while (!queue.empty());

	return output;
}

std::list<MatrixGrayScale> findFigures(
		const MatrixNormalizedRGB &image,
		real coverage = .01,
		const WekselNormalizedRGB &tolerance = WekselNormalizedRGB(0, 0, 0)
) {
	auto size = image.size();
	std::list<MatrixGrayScale> output;

	auto rng = cv::RNG(time(NULL));

	for (int i = 0; i < size.height * size.width * coverage; i++) {

		auto point = cv::Point2i(
				rng.uniform(0, size.width - 1),
				rng.uniform(0, size.height - 1)
		);

		// sprawdzamy czy punkt jest już w innej figurze
		for (const auto &prev : output) {
			if (prev(point) == WekselGrayScale(1)) {
				goto endloop;
			}
		}

		output.push_back(findFigure(image, point));

		endloop:
		int qq;
	}

	return output;
}

real countPerimeter(const MatrixGrayScale &img) {
	assertMatrixGrayScale(img);
	auto size = img.size();
	std::atomic<int> halfs = 0, squares = 0;

	img.forEach([&](const WekselGrayScale &weksel, const int position[]) -> void {

		if (weksel[0] < 0.5) {
			return;
		}

		auto isPerimeterToo = [&](const int &dx, const int &dy) -> bool {
			// tu jest na odwrót, ale to dobrze,
			auto next = cv::Point(position[1] + dx, position[0] + dy);
			return isPointInSize(next, size) && img(next)[0] >= 0.5;
		};

		if (isPerimeterToo(-1, -1)) ++squares;
		if (isPerimeterToo(+1, -1)) ++squares;
		if (isPerimeterToo(+1, +1)) ++squares;
		if (isPerimeterToo(-1, +1)) ++squares;

		if (isPerimeterToo(0, -1)) ++halfs;
		if (isPerimeterToo(+1, 0)) ++halfs;
		if (isPerimeterToo(-1, 0)) ++halfs;
		if (isPerimeterToo(0, +1)) ++halfs;
	});

	return halfs / 2 + squares * cv::sqrt(2) / 2;
}

real countArea(const MatrixGrayScale &img) {
	assertMatrixGrayScale(img);
	std::atomic<int> sum = 0;


	img.forEach([&](const WekselGrayScale &weksel, const int position[]) -> void {
		if (weksel[0] >= 0.5)
			sum += 1;
	});

	return sum;
}

int main_lab3(const char *img_path, bool angle = false) {
	auto title = std::string(img_path).substr(10);
	std::cout << "image: " << title << std::endl;

	const auto image = cv::imread(img_path);

	if (image.empty()) {
		std::cerr << "Error during reading " << img_path << std::endl;
		return 1;
	}

	const auto image_normalized = normalizeMatOfVec3bToMatOfVec3f(image);


	auto figures = findFigures(image_normalized);
	auto image2 = edgeFilter(image_normalized);

	// usuwamy tło
	figures.erase(
			std::remove_if(figures.begin(), figures.end(), [](const MatrixGrayScale &img) -> bool {
				assertMatrixGrayScale(img);
				return img(0, 0) == WekselGrayScale(1);
			})
	);

	std::for_each(figures.begin(), figures.end(), [&](MatrixGrayScale &img) -> void {
		assertMatrixGrayScale(img);
		auto size = img.size();

		std::stringstream ss;
		ss << "    ";

		auto img_perimeter = my_filter2D<WekselGrayScale, real>(img, edge_filter_matrix);
		clampGrayScaleMatrix(img_perimeter);

		auto area = countArea(img);
		ss << "S=" << area << " ";

		auto perimeter = countPerimeter(img_perimeter);
		ss << "L=" << perimeter << " ";

		ss << "W3=" << param::W3(area, perimeter) << " ";
		ss << "M1=" << param::M1(img) << " ";
		ss << "M7=" << param::M7(img) << " ";

		real max_X = -1, min_X = size.width;
		real max_Y = -1, min_Y = size.height;
		for (int x = 0; x < size.width; ++x) {
			for (int y = 0; y < size.height; ++y) {
				if (img(cv::Point(x, y))[0] >= 0.5) {
					if (max_X < x) max_X = x;
					if (max_Y < y) max_Y = y;
					if (min_X > x) min_X = x;
					if (min_Y > y) min_Y = y;
				}
			}
		}

		auto geo_center = cv::Point2f((max_X + min_X) / 2, (max_Y + min_Y) / 2);
//		ss << "geo_center=" << geo_center << " ";

		if (angle) {
			auto dash_i = param::m_pq(img, 1, 0) / param::m_pq(img, 0, 0);
			auto dash_j = param::m_pq(img, 0, 1) / param::m_pq(img, 0, 0);
			auto img_center = cv::Point2f(dash_j, dash_i);
//			ss << "img_center=" << img_center << " ";

			auto direction = img_center - geo_center;
//			ss << "direction=" << direction << " ";

			static auto rel_vec = cv::Point(-1, 0);
			real angle = angleBetweenVectors(rel_vec, direction);
			ss << "R=" << angle * 180 / CV_PI << " ";
		}

		std::cout << ss.str() << std::endl;
	});


//	cv::imshow(title, image);
//	cv::imshow("image2", denormalizeMatOfVec3bToMatOfVec3f(image2));

	return 0;
}