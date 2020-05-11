#include <queue>
#include <list>
#include <numeric>
#include "types.h"
#include "asserts.h"

const std::string img_path = "../assets/strzalki_2.dib";


static const cv::Mat edge_filter_matrix = (
		cv::Mat_<float>(3, 3)
				<<
				0, -1, 0,
				-1, 4, -1,
				0, -1, 0
);

MatrixNormalizedRGB edgeFilter(MatrixNormalizedRGB &image) {
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
		float coverage = .01,
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

float countPerimeter(const MatrixGrayScale &img) {
	assertMatrixGrayScale(img);
	auto size = img.size();
	float halfs = 0, squares = 0, any = 0;

	for (int x = 0; x < size.width; ++x) {
		for (int y = 0; y < size.height; ++y) {
			auto point = cv::Point(x, y);

			if (img(point)[0] < WekselGrayScale(1)[0]) {


				continue;
			}
			any++;

			auto isPerimeterToo = [&](const int &dx, const int &dy) -> bool {
				auto next = point + cv::Point(dx, dy);
				return isPointInSize(next, size) && img(next)[0] < WekselGrayScale(1)[0];
			};

			if (isPerimeterToo(-1, -1)) ++squares;
			if (isPerimeterToo(+1, -1)) ++squares;
			if (isPerimeterToo(+1, +1)) ++squares;
			if (isPerimeterToo(-1, +1)) ++squares;

			if (isPerimeterToo(0, -1)) ++halfs;
			if (isPerimeterToo(+1, 0)) ++halfs;
			if (isPerimeterToo(-1, 0)) ++halfs;
			if (isPerimeterToo(0, +1)) ++halfs;
		}
	}

	std::cout << cv::sum(img) << std::endl;
	return halfs / 2 + squares * cv::sqrt(2) / 2;
}

int main_lab3(int argc, char *argv[]) {
	auto image = normalizeMatOfVec3bToMatOfVec3f(cv::imread(img_path));


	if (image.empty()) {
		std::cerr << "Error during reading " << img_path << std::endl;
		return 1;
	}

	cv::imshow("image1", denormalizeMatOfVec3bToMatOfVec3f(image));

	auto figures = findFigures(image);
	auto image2 = edgeFilter(image);

	// usuwamy tło
	figures.erase(
			std::remove_if(figures.begin(), figures.end(), [](const MatrixGrayScale &img) -> bool {
				assertMatrixGrayScale(img);
				return img(0, 0) == WekselGrayScale(1);
			})
	);

	int i = 0;
	std::for_each(figures.begin(), figures.end(), [&](MatrixGrayScale &img) -> void {
		assertMatrixGrayScale(img);
		auto my_i = ++i;

		std::stringstream ss;

		ss << "i=" << my_i << " ";

		cv::imshow("img" + std::to_string(my_i), img);

		auto img_perimeter = my_filter2D<WekselGrayScale, float>(img, edge_filter_matrix);

		cv::imshow("img_perimeter" + std::to_string(my_i), img_perimeter);

		auto perimeter = countPerimeter(img_perimeter);

		ss << perimeter << std::endl;

		std::cout << ss.str() << std::endl;
	});


	cv::imshow("image2", denormalizeMatOfVec3bToMatOfVec3f(image2));

	cv::waitKey(-1);
	return 0;
}