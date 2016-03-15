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
};