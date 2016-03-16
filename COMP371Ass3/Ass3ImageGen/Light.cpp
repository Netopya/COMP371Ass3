#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 colour)
{
	this->position = position;
	this->colour = colour;
}

const glm::vec3 Light::getPosition()
{
	return position;
}

const glm::vec3 Light::getColour()
{
	return colour;
}
