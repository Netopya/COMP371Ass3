#include "Plane.h"

#include <sstream>

Plane::Plane(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 normal, glm::vec3 position) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->normal = normal;
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
