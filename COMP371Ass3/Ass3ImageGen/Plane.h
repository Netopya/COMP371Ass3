#pragma once

#include "SceneObject.h"
#include "../glm/glm.hpp"

class Plane : public SceneObject{
private:
	glm::vec3 normal;
	glm::vec3 position;
public:
	Plane(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 normal, glm::vec3 position);
	string toString() override;
	float vecHit(glm::vec3 position, glm::vec3 vector) override;
};
