
#include "../glm/glm.hpp"
#include "../CImg-1.7.0_pre030916/CImg.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "SceneObject.h"
#include "Camera.h"
#include "Light.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

#define M_PI        3.14159265358979323846264338327950288   /* pi */
#define KINDA_SMALL_NUMBER 0.0001


/*
	COMP 371 Assignment 3 - Programming Assignment 3
	Custom ray tracer
	Defines object from an input file and draws the scene

	Features:
	Anti-aliasing
	Recursive reflections
*/



using namespace std;

const float REFLECTION_FACTOR = 0.1f;	// Intensity of reflections
const int NUM_REFLECTIONS = 10;			// Number of recursive reflections to perform
const bool SHOW_IMAGE = false;			// Whether to show the image in a window
const float AMBIENT_FACTOR = 0.1f;		// Intensity of the ambient colour on an unlit surface

int width = 800;
int height = 1500;

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

	// Iterate over the number of objects
	while (numOfObjects > 0)
	{
		numOfObjects--;

		string objectType;
		file >> objectType;

		string title; // Placeholder string to hold the labels

		// Create the appropriate object depending on the specified type
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

// Method to print out scene objects to the console
void debugSceneObjects() {
	for (unsigned i = 0; i < sceneObjects.size(); i++)
	{
		cout << sceneObjects[i]->toString() << endl << endl;
	}
	system("pause");
}

// Jitter a value around 0
float jitter(float value)
{
	value *= 0.8f;
	return ((rand() % 100) / 100.0f * value) - (value / 2.0f);
}

// Calculate all the rays to shoot out of the camera
void calculateRays() {
	
	float a = camera->getAspect_ratio();
	
	// Set the width based on the aspect ratio
	width = a * height;

	float focalLength = camera->getFocal_length();
	glm::vec3 cameraPos(camera->getPosition());
	glm::vec3 cop(camera->getCenterOfProjection());

	float opposite = focalLength * tan(camera->getTheta() * M_PI / 180.0f / 2.0f);

	// Calculate the position of the top left pixel
	glm::vec3 topLeft(-1.0f * a * opposite, opposite, 0);
	topLeft += camera->getPosition();

	float scenePixelHeight = 2.0f * opposite;
	float scenePixelWidth = a * scenePixelHeight;

	float pixelHeight = scenePixelHeight / height;
	float pixelWidth = scenePixelWidth / width;

	float aaHeight = pixelHeight / 4;
	float aaWidth = pixelWidth / 4;

	// Find a value to jitter anti-aliasing samples by
	float jitterH = jitter(aaHeight);
	float jitterW = jitter(aaWidth);

	cameraRays.reserve(height);

	for (int i = 0; i < height; i++)
	{
		cameraRays.push_back(new vector<vector<glm::vec3*>*>());
		
		cameraRays[i]->reserve(width);

		for (int j = 0; j < width; j++)
		{
			cameraRays[i]->push_back(new vector<glm::vec3*>);
			
			cameraRays[i]->at(j)->reserve(4);

			// Calculate the position of each pixel
			float y = topLeft.y - (i*pixelHeight) - (pixelHeight / 2.0f);
			float x = topLeft.x + (j*pixelWidth) + (pixelWidth / 2.0f);

			// Sample each pixel four times
			// Split each pixel into four quadrants with each sample jittered within its quadrant
			glm::vec3 pixelPosition1(x - aaWidth + jitterW, y - aaHeight + jitterH, cameraPos.z);
			glm::vec3 pixelPosition2(x + aaWidth + jitterW, y - aaHeight + jitterH, cameraPos.z);
			glm::vec3 pixelPosition3(x - aaWidth + jitterW, y + aaHeight + jitterH, cameraPos.z);
			glm::vec3 pixelPosition4(x + aaWidth + jitterW, y + aaHeight + jitterH, cameraPos.z);

			// Save the rays
			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition1 - cop)));
			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition2 - cop)));
			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition3 - cop)));
			cameraRays[i]->at(j)->push_back(new glm::vec3(glm::normalize(pixelPosition4 - cop)));
		}
	}
}

// Shoot a ray a determine the colour
glm::vec3 shootRay(glm::vec3 position, glm::vec3 ray, int iterations)
{
	float mint = -1;
	int foundK = -1;
	glm::vec3 at;
	
	// Search for the nearest object hit
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

	glm::vec3 illumination;

	// If an object was found
	if (foundK >= 0)
	{
		// Give it some ambient light
		illumination += sceneObjects[foundK]->getAmbient() * AMBIENT_FACTOR;

		// Normal at point hit
		glm::vec3 normal = sceneObjects[foundK]->getNormalAtPoint(at);
		bool lit = false;

		// Go through each light source
		for (unsigned m = 0; m < lights.size(); m++)
		{
			bool blocked = false;

			// Move the point above the surface slightly
			at += normal * (float)KINDA_SMALL_NUMBER;

			// Calculate the vector pointing to the light source
			glm::vec3 lightRay = glm::normalize(lights[m]->getPosition() - at);
			
			// Determine if something is blocking the light source
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
				lit = true;

				// Calculate the phong illumination model and add it to the illumination

				glm::vec3 reflectionV(glm::reflect(lightRay, normal));

				float diffuseVector = glm::dot(lightRay, normal);
				diffuseVector = diffuseVector < 0 ? 0 : diffuseVector;
				float specularVector = glm::dot(reflectionV, ray);
				specularVector = specularVector < 0 ? 0 : specularVector;


				illumination += lights[m]->getColour() * (sceneObjects[foundK]->getDiffuse() * diffuseVector + sceneObjects[foundK]->getSpecular() * pow(specularVector, sceneObjects[foundK]->getShininess()));
			}
		}		

		// Continue iterating to create a reflection
		if (iterations > 0)
		{
			illumination += REFLECTION_FACTOR * shootRay(at, glm::reflect(ray, normal), --iterations);
		}

		return illumination;
	}
	else
	{
		// Return an empty vector if nothing found
		return glm::vec3();
	}
}

int main() {
	cout << "Hello World";

	/* initialize random seed: */
	srand(time(NULL));

	// Go through all specified scene files
	for (int l = 0; l < numFiles; l++)
	{
		// Read file, skip if there is an error
		if (readInputFile(files[l]) > 0)
		{
			break;
		}


		cout << "Processing rays" << endl;

		// Calculate camera rays
		calculateRays();

		//Creates an image with three channels and sets it to black
		cimg_library::CImg<float> image(width, height, 1, 3, 0);

		// Get the center of projection
		glm::vec3 cop(camera->getCenterOfProjection());

		// Go through all pixels and find the colour at that points
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				glm::vec3 illumination;

				// Go through a pixel multiple times for anti aliasing
				for (int k = 0; k < cameraRays[i]->at(j)->size(); k++)
				{
					glm::vec3 vectorRay(*cameraRays[i]->at(j)->at(k));

					illumination += shootRay(cop, vectorRay, NUM_REFLECTIONS);
				}

				// Average colour of anti-aliasing
				illumination /= cameraRays[i]->at(j)->size();

				// Set the colour of the pixel
				image(j, i, 0, 0) = 255 * illumination.x;
				image(j, i, 0, 1) = 255 * illumination.y;
				image(j, i, 0, 2) = 255 * illumination.z;

			}
		}
		

		if (SHOW_IMAGE)
		{
			//Display the rendered image on screen
			cimg_library::CImgDisplay main_disp(image, "Render");
			while (!main_disp.is_closed())
				main_disp.wait();
		}		

		// Save the image to a file
		image.normalize(0, 255);
		image.save((files[l] + ".bmp").c_str());


		cout << "Clearing memory, please wait" << endl;

		// Delete all objects
		delete camera;

		for (unsigned i = 0; i < sceneObjects.size(); i++)
		{
			delete (sceneObjects[i]);
		}

		sceneObjects.clear();

		for (unsigned i = 0; i < lights.size(); i++)
		{
			delete (lights[i]);
		}

		lights.clear();


		for (unsigned i = 0; i < cameraRays.size(); i++)
		{
			for (unsigned j = 0; j < cameraRays[i]->size(); j++)
			{
				for (unsigned k = 0; k < cameraRays[i]->at(j)->size(); k++)
				{
					delete(cameraRays[i]->at(j)->at(k));
				}
				delete(cameraRays[i]->at(j));
			}
			delete(cameraRays[i]);
		}

		cameraRays.clear();
	}


	return 0;
}
