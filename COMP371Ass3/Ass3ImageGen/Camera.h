#pragma once

#include "../glm/glm.hpp"
#include <vector>

#define M_PI        3.14159265358979323846264338327950288   /* pi */

using namespace std;

/*
	Class to hold camera information
*/

class Camera {
private:
	glm::vec3 position;
	float theta;
	float focal_length;
	float aspect_ratio;
	
	int height;
	int width;
	glm::vec3 topLeft;
	float scenePixelHeight;
	float scenePixelWidth;
	float pixelHeight;
	float pixelWidth;
	float aaHeight;
	float aaWidth;
	float jitterH;
	float jitterW;

	float jitter(float value);
public:
	Camera(glm::vec3 position, float theta, float focal_length, float aspect_ratio, int height);
	const glm::vec3 getPosition();
	const float getTheta();
	const float getFocal_length();
	const float getAspect_ratio();
	const glm::vec3 getCenterOfProjection();
	vector<glm::vec3*> getRays(int y, int x);
	const int getWidth();
};