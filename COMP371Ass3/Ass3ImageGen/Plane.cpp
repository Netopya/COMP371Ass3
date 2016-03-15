#include "Plane.h"

Plane::Plane(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 normal, glm::vec3 position) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->normal = normal;
	this->position = position;
}
