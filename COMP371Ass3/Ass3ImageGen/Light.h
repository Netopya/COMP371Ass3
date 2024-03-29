#pragma once

#include "../glm/glm.hpp"

/*
	Class to hold light information
*/

class Light {
private:
	glm::vec3 position;
	glm::vec3 colour;

public:
	Light(glm::vec3 position, glm::vec3 colour);
	const glm::vec3 getPosition();
	const glm::vec3 getColour();
};