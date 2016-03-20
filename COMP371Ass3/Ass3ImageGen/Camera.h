#pragma once

#include "../glm/glm.hpp"

/*
	Class to hold camera information
*/

class Camera {
private:
	glm::vec3 position;
	float theta;
	float focal_length;
	float aspect_ratio;

public:
	Camera(glm::vec3 position, float theta, float focal_length, float aspect_ratio);
	const glm::vec3 getPosition();
	const float getTheta();
	const float getFocal_length();
	const float getAspect_ratio();
	const glm::vec3 getCenterOfProjection();
};