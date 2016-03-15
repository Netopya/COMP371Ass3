#include "Sphere.h"

Sphere::Sphere(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 position, float radius) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->position = position;
	this->radius = radius;
}
