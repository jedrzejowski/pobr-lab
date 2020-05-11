#include <queue>
#include <list>
#include "types.h"

const std::string img_path = "../assets/kolo.dib";


NormalizedImage edgeFilter(NormalizedImage &image) {
	static const cv::Mat edge_y_filter_matrix = (
			cv::Mat_<float>(3, 3)
					<<
					0, -1, 0,
					-1, 4, -1,
					0, -1, 0
	);

	return my_filter2D(image, edge_y_filter_matrix);
}

MatrixGrayScale findFigure(
		const NormalizedImage &image,
		const cv::Point2i &seed,
		const NormalizedPixel &tolerance = NormalizedPixel(0, 0, 0)
) {

	const auto size = image.size();
	cv::Mat done = cv::Mat::zeros(size, CV_8U);
	MatrixGrayScale output = MatrixGrayScale::zeros(size);

	struct Step {
		cv::Point point;
		NormalizedPixel color;
	};

	auto queue = std::queue<Step>();
	auto point = seed;

	auto push = [&](const cv::Point &point, const NormalizedPixel &pixel) -> void {
		if (point.x < 0 || point.x >= size.width || point.y < 0 || point.y >= size.height) {
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
		const NormalizedImage &image,
		float coverage = .01,
		const NormalizedPixel &tolerance = NormalizedPixel(0, 0, 0)
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
			if (prev(point) == 1) {
				goto endloop;
			}
		}

		output.push_back(findFigure(image, point));

		endloop:
		int qq;
	}

	return output;
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

//	// usuwamy tło
	figures.erase(
			std::remove_if(figures.begin(), figures.end(), [](const MatrixGrayScale &img) -> bool {
				return img.at<WekselGrayScale>(0, 0) != 1;
			})
	);

	int i = 0;
	for (const auto &fig : figures) {
		i++;
		cv::imshow("fig" + std::to_string(i), denormalizeMatOfVec3bToMatOfVec3f(fig));
	}

	cv::imshow("image2", denormalizeMatOfVec3bToMatOfVec3f(image2));

	cv::waitKey(-1);
	return 0;
}