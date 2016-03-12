
#include "../glm/glm.hpp"
#include "../CImg-1.7.0_pre030916/CImg.h"
#include <iostream>

using namespace std;
const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
	cout << "Hello World";

	//Creates an image with three channels and sets it to black
	cimg_library::CImg<float> image(WIDTH, HEIGHT, 1, 3, 0);

	//Display the rendered image on screen
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed())
		main_disp.wait();

	return 0;
}