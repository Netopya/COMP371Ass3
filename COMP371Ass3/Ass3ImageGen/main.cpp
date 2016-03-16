
#include "../glm/glm.hpp"
#include "../CImg-1.7.0_pre030916/CImg.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include "SceneObject.h"
#include "Camera.h"
#include "Light.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

using namespace std;
int width = 800;
int height = 600;

ifstream file;

Camera* camera;
vector<SceneObject*> sceneObjects;
vector<Light*> lights;
vector<vector<glm::vec3*>*> cameraRays;

const string files[] = {
	"scene1.txt",
	"scene2.txt",
	"scene3.txt",
	"scene4.txt",
	"scene5.txt",
	"scene6.txt",
	"scene7.txt"
};

const int numFiles = 7;

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

	return 0;
}

void debugSceneObjects() {
	for (unsigned i = 0; i < sceneObjects.size(); i++)
	{
		cout << sceneObjects[i]->toString() << endl << endl;
	}
	system("pause");
}

void calculateRays() {
	width = camera->getAspect_ratio() * height;
	float a = camera->getAspect_ratio();

	glm::vec3 cameraPos(camera->getPosition());
	glm::vec3 cop(cameraPos);
	cop.z -= camera->getFocal_length();

	float opposite = tan(camera->getTheta() / 2.0f);

	glm::vec3 topLeft(-1.0f * a * opposite, opposite, 0);
	glm::vec3 topRight(a * opposite, opposite, 0);
	glm::vec3 bottomLeft(-1.0f * a * opposite, -1.0f * opposite, 0);
	glm::vec3 bottomRight(a * opposite, -1.0f * opposite, 0);
	topLeft += camera->getPosition();


	float scenePixelHeight = 2.0f * opposite;
	float scenePixelWidth = a * scenePixelHeight;

	float pixelHeight = scenePixelHeight / height;
	float pixelWidth = scenePixelWidth / width;

	float heightFactor = scenePixelHeight / height;
	float widthFactor = scenePixelWidth / width;

	for (int i = 0; i < height; i++)
	{
		cameraRays.push_back(new vector<glm::vec3*>());

		for (int j = 0; j < width; j++)
		{
			float y = topLeft.y - (i*heightFactor) - (pixelHeight / 2);
			float x = topLeft.x + (j*widthFactor) + (pixelWidth / 2);
			glm::vec3 pixelPosition(x, y, cameraPos.z);

			cameraRays[i]->push_back(new glm::vec3(glm::normalize(pixelPosition - cop)));
		}
	}
}

int main() {
	cout << "Hello World";



	while (true)
	{

		if (readInputFile() > 0)
		{
			break;
		}

		file.close();

		calculateRays();

		//Creates an image with three channels and sets it to black
		cimg_library::CImg<float> image(width, height, 1, 3, 0);


		glm::vec3 cameraPos(camera->getPosition());
		glm::vec3 cop(cameraPos);
		cop.z -= camera->getFocal_length();

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				glm::vec3 vectorRay(*cameraRays[i]->at(j));

				float mint = -1;

				for (unsigned k = 0; k < sceneObjects.size(); k++)
				{
					float newt = sceneObjects[k]->vecHit(cop, vectorRay);
					if (newt >= 0 && (mint < 0 || newt < mint))
					{

						image(j, height - 1 - i, 0, 0) = 255 * sceneObjects[k]->getAmbient().x;
						image(j, height - 1 - i, 0, 1) = 255 * sceneObjects[k]->getAmbient().y;
						image(j, height - 1 - i, 0, 2) = 255 * sceneObjects[k]->getAmbient().z;

					}
				}

			}
		}

		//image(100, 100, 0, 2, 255);

		//debugSceneObjects();	

		//Display the rendered image on screen
		cimg_library::CImgDisplay main_disp(image, "Render");
		while (!main_disp.is_closed())
			main_disp.wait();
	}


	return 0;
}
