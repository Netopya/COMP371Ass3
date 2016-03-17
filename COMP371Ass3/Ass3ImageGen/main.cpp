
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

#define M_PI        3.14159265358979323846264338327950288   /* pi */
#define KINDA_SMALL_NUMBER 0.0001

using namespace std;
int width = 800;
int height = 600;

float globalPixelHeight;
float globalPixelWidth;

ifstream file;

Camera* camera;
vector<SceneObject*> sceneObjects;
vector<Light*> lights;
vector<vector<vector<glm::vec3*>*>*> cameraRays;

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

int readInputFile(string inputFile) {

	string filename;

	if (inputFile == "")
	{
		// Ask the user for the input file
		cout << "Enter the name of the file" << endl;


		cin >> filename;
	}
	else
	{
		filename = inputFile;
	}
	

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

	file.close();

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
	
	float a = camera->getAspect_ratio();
	
	width = a * height;

	/*
	float focalLength = 500.0f;//camera->getFocal_length();
	glm::vec3 cameraPos(camera->getPosition());
	glm::vec3 cop(cameraPos);
	cameraPos.z += focalLength;
	*/

	float focalLength = camera->getFocal_length();
	glm::vec3 cameraPos(camera->getPosition());
	glm::vec3 cop(camera->getCenterOfProjection());
	//cameraPos.z += focalLength;

	float opposite = focalLength * tan(camera->getTheta() * M_PI / 180.0f / 2.0f);

	glm::vec3 topLeft(-1.0f * a * opposite, opposite, 0);
	//glm::vec3 topRight(a * opposite, opposite, 0);
	//glm::vec3 bottomLeft(-1.0f * a * opposite, -1.0f * opposite, 0);
	//glm::vec3 bottomRight(a * opposite, -1.0f * opposite, 0);
	topLeft += camera->getPosition();


	float scenePixelHeight = 2.0f * opposite;
	float scenePixelWidth = a * scenePixelHeight;

	float pixelHeight = scenePixelHeight / height;
	float pixelWidth = scenePixelWidth / width;

	globalPixelHeight = pixelHeight;
	globalPixelWidth = pixelWidth;

	float heightFactor = scenePixelHeight / height;
	float widthFactor = scenePixelWidth / width;

	for (int i = 0; i < height; i++)
	{
		cameraRays.push_back(new vector<vector<glm::vec3*>*>());

		for (int j = 0; j < width; j++)
		{
			cameraRays[i]->push_back(new vector<glm::vec3*>);

			float y = topLeft.y - (i*heightFactor) - (pixelHeight / 2);
			float x = topLeft.x + (j*widthFactor) + (pixelWidth / 2);

			float aaHeight = pixelHeight / 4;
			float aaWidth = pixelWidth / 4;

			glm::vec3 pixelPosition1(x - aaWidth, y - aaHeight, cameraPos.z);
			glm::vec3 pixelPosition2(x + aaWidth, y - aaHeight, cameraPos.z);
			glm::vec3 pixelPosition3(x - aaWidth, y + aaHeight, cameraPos.z);
			glm::vec3 pixelPosition4(x + aaWidth, y + aaHeight, cameraPos.z);

			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition1 - cop)));
			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition2 - cop)));
			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition3 - cop)));
			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition4 - cop)));
		}
	}
}

glm::vec3 shootRay(glm::vec3 position, glm::vec3 ray)
{
	float mint = -1;
	int foundK = -1;
	glm::vec3 at;
	for (unsigned k = 0; k < sceneObjects.size(); k++)
	{
		float newt = sceneObjects[k]->vecHit(position, ray);
		if (newt >= 0 && (mint < 0 || newt < mint))
		{
			foundK = k;
			mint = newt;
			at = position + newt*ray;
		}
	}

	glm::vec3 illumination;// (sceneObjects[foundK]->getAmbient() * 0.5f);

	if (foundK >= 0)
	{

		glm::vec3 normal = sceneObjects[foundK]->getNormalAtPoint(at);

		for (unsigned m = 0; m < lights.size(); m++)
		{
			bool blocked = false;

			at += normal * (float)KINDA_SMALL_NUMBER;

			glm::vec3 lightRay = glm::normalize(lights[m]->getPosition() - at);



			for (unsigned k = 0; k < sceneObjects.size(); k++)
			{
				float newt = sceneObjects[k]->vecHit(at, lightRay);
				if (newt >= 0)
				{
					blocked = true;
					break;
				}
			}

			if (!blocked) {
				// http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
				//glm::vec3 reflectionV(ray - 2.0f * (glm::dot(ray, normal) * normal));
				glm::vec3 reflectionV(glm::reflect(ray, normal));

				float diffuseVector = glm::dot(lightRay, normal);
				diffuseVector = diffuseVector < 0 ? 0 : diffuseVector;
				float specularVector = glm::dot(reflectionV, ray);
				specularVector = specularVector < 0 ? 0 : specularVector;


				illumination += lights[m]->getColour() * (sceneObjects[foundK]->getDiffuse() * diffuseVector + sceneObjects[foundK]->getSpecular() * pow(specularVector, sceneObjects[foundK]->getShininess()));
			}
		}


		return illumination;
		//image(j, i, 0, 0) = 255 * sceneObjects[foundK]->getAmbient().x;
		//image(j, i, 0, 1) = 255 * sceneObjects[foundK]->getAmbient().y;
		//image(j, i, 0, 2) = 255 * sceneObjects[foundK]->getAmbient().z;




	}
	else
	{
		return glm::vec3();
	}
}

int main() {
	cout << "Hello World";



	for (int l = 0; l < numFiles; l++)
	{

		if (readInputFile(files[l]) > 0)
		{
			break;
		}

		debugSceneObjects();

		calculateRays();

		//Creates an image with three channels and sets it to black
		cimg_library::CImg<float> image(width, height, 1, 3, 0);


		glm::vec3 cop(camera->getCenterOfProjection());

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//glm::vec3 vectorRay(*cameraRays[i]->at(j));

				//glm::vec3 illumination(shootRay(cop, vectorRay));

				glm::vec3 illumination;

				for (int k = 0; k < cameraRays[i]->at(j)->size(); k++)
				{
					glm::vec3 vectorRay(*cameraRays[i]->at(j)->at(k));

					illumination += shootRay(cop, vectorRay);
				}

				//illumination += shootRay(cop, glm::vec3(vectorRay.x - globalPixelWidth / 4.0f, vectorRay.y - globalPixelHeight / 4.0f, vectorRay.z));
				//illumination += shootRay(cop, glm::vec3(vectorRay.x + globalPixelWidth / 4.0f, vectorRay.y - globalPixelHeight / 4.0f, vectorRay.z));
				//illumination += shootRay(cop, glm::vec3(vectorRay.x - globalPixelWidth / 4.0f, vectorRay.y + globalPixelHeight / 4.0f, vectorRay.z));
				//illumination += shootRay(cop, glm::vec3(vectorRay.x + globalPixelWidth / 4.0f, vectorRay.y + globalPixelHeight / 4.0f, vectorRay.z));

				illumination /= cameraRays[i]->at(j)->size();

				image(j, i, 0, 0) = 255 * illumination.x;
				image(j, i, 0, 1) = 255 * illumination.y;
				image(j, i, 0, 2) = 255 * illumination.z;

			}
		}

		//image(100, 100, 0, 2, 255);

		//debugSceneObjects();	

		//Display the rendered image on screen
		cimg_library::CImgDisplay main_disp(image, "Render");
		while (!main_disp.is_closed())
			main_disp.wait();

		delete camera;
		sceneObjects.clear();

		lights.clear();


		cameraRays.clear();
	}


	return 0;
}
