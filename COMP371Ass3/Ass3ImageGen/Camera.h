#pragma once

#include "../glm/glm.hpp"

class Camera {
private:
	glm::vec3 position;
	float theta;
	float focal_length;
	float aspect_ratio;

public:
	Camera(glm::vec3 position, float theta, float focal_length, float aspect_ratio);
	void calculateRays();
	const glm::vec3 getPosition();
	const float getTheta();
	const float getFocal_length();
	const float getAspect_ratio();
};