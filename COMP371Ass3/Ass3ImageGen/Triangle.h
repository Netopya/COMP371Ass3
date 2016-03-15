#pragma once

#include "SceneObject.h"
#include "../glm/glm.hpp"

class Triangle : public SceneObject {
private:
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 vertex3;
public:
	Triangle(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3);
};