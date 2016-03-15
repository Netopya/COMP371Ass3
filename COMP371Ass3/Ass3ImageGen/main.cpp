
#include "../glm/glm.hpp"
#include "../CImg-1.7.0_pre030916/CImg.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "SceneObject.h"
#include "Camera.h"
#include "Light.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

using namespace std;
const int WIDTH = 800;
const int HEIGHT = 600;

ifstream file;

Camera* camera;
vector<SceneObject*> sceneObjects;
vector<Light*> lights;

int readInputFile() {
	// Ask the user for the input file
	cout << "Enter the name of the file" << endl;

	string filename;
	cin >> filename;

	file.open(filename, ios::in);

	if (!file.is_open())
	{
		cout << "File name is invalid";
		system("pause");
		return 1;
	}

	int numOfObjects;
	file >> numOfObjects;

	while (numOfObjects > 0)
	{
		numOfObjects--;

		string objectType;
		file >> objectType;

		string title;

		if (objectType == "camera")
		{		
			glm::vec3 position;
			float fov;
			float focal_length;
			float ascpect_ratio;

			file >> title >> position.x >> position.y >> position.z;
			file >> title >> fov >> title >> focal_length >> title >> ascpect_ratio;

			camera = new Camera(position, fov, focal_length, ascpect_ratio);
		}
		else if (objectType == "triangle")
		{
			glm::vec3 v1, v2, v3;
			glm::vec3 amb, dif, spe;
			float shi;

			file >> title >> v1.x >> v1.y >> v1.z;
			file >> title >> v2.x >> v2.y >> v2.z;
			file >> title >> v3.x >> v3.y >> v3.z;

			file >> title >> amb.x >> amb.y >> amb.z;
			file >> title >> dif.x >> dif.y >> dif.z;
			file >> title >> spe.x >> spe.y >> spe.z;
			file >> title >> shi;

			sceneObjects.push_back(new Triangle(amb, dif, spe, shi, v1, v2, v3));
		}
		else if (objectType == "sphere")
		{
			glm::vec3 pos;
			float radius;
			glm::vec3 amb, dif, spe;
			float shi;

			file >> title >> pos.x >> pos.y >> pos.z;
			file >> title >> radius;

			file >> title >> amb.x >> amb.y >> amb.z;
			file >> title >> dif.x >> dif.y >> dif.z;
			file >> title >> spe.x >> spe.y >> spe.z;
			file >> title >> shi;

			sceneObjects.push_back(new Sphere(amb, dif, spe, shi, pos, radius));
		}
		else if (objectType == "plane")
		{
			glm::vec3 nor, pos;
			glm::vec3 amb, dif, spe;
			float shi;

			file >> title >> nor.x >> nor.y >> nor.z;
			file >> title >> pos.x >> pos.y >> pos.z;

			file >> title >> amb.x >> amb.y >> amb.z;
			file >> title >> dif.x >> dif.y >> dif.z;
			file >> title >> spe.x >> spe.y >> spe.z;
			file >> title >> shi;

			sceneObjects.push_back(new Plane(amb, dif, spe, shi, nor, pos));
		}
		else if (objectType == "light")
		{
			glm::vec3 pos, col;

			file >> title >> pos.x >> pos.y >> pos.z;
			file >> title >> col.x >> col.y >> col.z;

			lights.push_back(new Light(pos, col));
		}
	}
}

void debugSceneObjects() {
	for (unsigned i = 0; i < sceneObjects.size(); i++)
	{
		cout << sceneObjects[i]->toString() << endl << endl;
	}
	system("pause");
}

int main() {
	cout << "Hello World";

	//Creates an image with three channels and sets it to black
	cimg_library::CImg<float> image(WIDTH, HEIGHT, 1, 3, 0);

	readInputFile();

	debugSceneObjects();	

	//Display the rendered image on screen
	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed())
		main_disp.wait();

	return 0;
}
