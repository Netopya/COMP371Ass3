#pragma once

#include "SceneObject.h"
#include "../glm/glm.hpp"

class Sphere : public SceneObject {
private:
	glm::vec3 position;
	float radius;
public:
	Sphere(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 position, float radius);
	string toString() override;
};