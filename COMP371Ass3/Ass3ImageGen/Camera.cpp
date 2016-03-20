#include "Camera.h"

Camera::Camera(glm::vec3 position, float theta, float focal_length, float aspect_ratio, int height)
{
	this->position = position;
	this->theta = theta;
	this->focal_length = focal_length;
	this->aspect_ratio = aspect_ratio;

	this->height = height;
	width = aspect_ratio * height;

	float opposite = focal_length * tan(theta * M_PI / 180.0f / 2.0f);
	topLeft.x = -1.0f * aspect_ratio * opposite;
	topLeft.y = opposite;
	topLeft += getPosition();

	scenePixelHeight = 2.0f * opposite;
	scenePixelWidth = aspect_ratio * scenePixelHeight;

	pixelHeight = scenePixelHeight / height;
	pixelWidth = scenePixelWidth / width;

	aaHeight = pixelHeight / 4;
	aaWidth = pixelWidth / 4;

	// Find a value to jitter anti-aliasing samples by
	jitterH = jitter(aaHeight);
	jitterW = jitter(aaWidth);
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

vector<glm::vec3*> Camera::getRays(int y, int x)
{
	vector<glm::vec3*> rays;
	rays.reserve(4);

	// Calculate the position of each pixel
	float py = topLeft.y - (y*pixelHeight) - (pixelHeight / 2.0f);
	float px = topLeft.x + (x*pixelWidth) + (pixelWidth / 2.0f);

	glm::vec3 cameraPos(getPosition());
	glm::vec3 cop(getCenterOfProjection());

	// Sample each pixel four times
	// Split each pixel into four quadrants with each sample jittered within its quadrant
	glm::vec3 pixelPosition1(px - aaWidth + jitterW, py - aaHeight + jitterH, cameraPos.z);
	glm::vec3 pixelPosition2(px + aaWidth + jitterW, py - aaHeight + jitterH, cameraPos.z);
	glm::vec3 pixelPosition3(px - aaWidth + jitterW, py + aaHeight + jitterH, cameraPos.z);
	glm::vec3 pixelPosition4(px + aaWidth + jitterW, py + aaHeight + jitterH, cameraPos.z);

	// Save the rays
	rays.push_back(new glm::vec3(glm::normalize(pixelPosition1 - cop)));
	rays.push_back(new glm::vec3(glm::normalize(pixelPosition2 - cop)));
	rays.push_back(new glm::vec3(glm::normalize(pixelPosition3 - cop)));
	rays.push_back(new glm::vec3(glm::normalize(pixelPosition4 - cop)));

	return rays;
}

const int Camera::getWidth()
{
	return width;
}

float Camera::jitter(float value)
{
	value *= 0.8f;
	return ((rand() % 100) / 100.0f * value) - (value / 2.0f);
}