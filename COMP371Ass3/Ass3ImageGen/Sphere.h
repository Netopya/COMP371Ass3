#pragma once

#include "SceneObject.h"
#include "../glm/glm.hpp"

/*
	Class for a sphere object
*/

class Sphere : public SceneObject {
private:
	glm::vec3 position;
	float radius;
public:
	Sphere(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 position, float radius);
	string toString() override;
	float vecHit(glm::vec3 position, glm::vec3 vector) override;
	glm::vec3 getNormalAtPoint(glm::vec3 point) override;
};