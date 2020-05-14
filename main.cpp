#include "lab2.h"
#include "lab3.h"
#include <string>

int main(int argc, char *argv[]) {
//	main_lab2(argc, argv);

	main_lab3("../assets/elipsa.dib");
	main_lab3("../assets/elipsa1.dib");
	main_lab3("../assets/kolo.dib");
	main_lab3("../assets/prost.dib");
	main_lab3("../assets/troj.dib");
	main_lab3("../assets/strzalki_1.dib", true);
	main_lab3("../assets/strzalki_2.dib", true);

	cv::waitKey(-1);
	return 0;
}