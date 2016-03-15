#include "Sphere.h"

#include <sstream>

Sphere::Sphere(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 position, float radius) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->position = position;
	this->radius = radius;
}

string Sphere::toString()
{
	std::stringstream ss;
	ss << "I am a sphere" << endl;
	ss << SceneObject::toString() << endl;
	ss << "Position: " << position.x << ' ' << position.y << ' ' << position.z << endl;
	ss << "Radius: " << radius << endl;

	return ss.str();
}
