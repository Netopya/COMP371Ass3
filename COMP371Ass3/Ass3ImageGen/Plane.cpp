#include "Plane.h"

#include <sstream>

Plane::Plane(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 normal, glm::vec3 position) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->normal = glm::normalize(normal);
	this->position = position;
}

string Plane::toString()
{
	std::stringstream ss;
	ss << "I am a plane" << endl;
	ss << SceneObject::toString() << endl;
	ss << "Normal: " << normal.x << ' ' << normal.y << ' ' << normal.z << endl;
	ss << "Position: " << position.x << ' ' << position.y << ' ' << position.z << endl;
	
	return ss.str();
}

float Plane::vecHit(glm::vec3 position, glm::vec3 vector)
{
	// Whether a vector hits a plane

	float dom = glm::dot(normal, vector);

	if (dom == 0)
	{
		return false;
	}

	float t = (glm::dot(normal, this->position) - glm::dot(normal, position)) / dom;

	// Return the factor with which to multiply the vector in order to reach the hit location
	return t;

}

glm::vec3 Plane::getNormalAtPoint(glm::vec3 point)
{
	return normal;
}
