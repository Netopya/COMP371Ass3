#pragma once

#include "../glm/glm.hpp"
#include <string>

using namespace std;

class SceneObject {
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
public:
	SceneObject(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	virtual string toString();
	virtual float vecHit(glm::vec3 position, glm::vec3 vector);
	const glm::vec3 getAmbient();
	const glm::vec3 getDiffuse();
	const glm::vec3 getSpecular();
	const float getShininess();
	virtual glm::vec3 getNormalAtPoint(glm::vec3 point);
};