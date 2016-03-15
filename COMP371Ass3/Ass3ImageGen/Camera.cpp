#include "Camera.h"

Camera::Camera(glm::vec3 position, float theta, float focal_length, float aspect_ratio)
{
	this->position = position;
	this->theta = theta;
	this->focal_length = focal_length;
	this->aspect_ratio = aspect_ratio;
}
