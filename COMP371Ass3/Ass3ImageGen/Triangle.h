#pragma once

#include "SceneObject.h"
#include "../glm/glm.hpp"

/*
	Class to hold triangle information defined by four points
*/

class Triangle : public SceneObject {
private:
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 vertex3;
	glm::vec3 normal;

	float sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
public:
	Triangle(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3);
	string toString() override;
	
	float vecHit(glm::vec3 position, glm::vec3 vector) override;
	glm::vec3 getNormalAtPoint(glm::vec3 point) override;
};