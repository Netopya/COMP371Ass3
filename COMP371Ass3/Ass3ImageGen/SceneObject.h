#pragma once

#include "../glm/glm.hpp"

class SceneObject {
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
public:
	SceneObject(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
};