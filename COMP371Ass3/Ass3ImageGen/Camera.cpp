#include "Camera.h"

Camera::Camera(glm::vec3 position, float theta, float focal_length, float aspect_ratio)
{
	this->position = position;
	this->theta = theta;
	this->focal_length = focal_length;
	this->aspect_ratio = aspect_ratio;
}

const glm::vec3 Camera::getPosition()
{
	// The camera is located infront of the center of projection
	return glm::vec3(position.x, position.y, position.z - focal_length);
}

const float Camera::getTheta()
{
	return theta;
}

const float Camera::getFocal_length()
{
	return focal_length;
}

const float Camera::getAspect_ratio()
{
	return aspect_ratio;
}

const glm::vec3 Camera::getCenterOfProjection()
{
	return position;
}
