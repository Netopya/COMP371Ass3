#include "Triangle.h"

#include <sstream>

Triangle::Triangle(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
	this->vertex3 = vertex3;
}

string Triangle::toString()
{
	std::stringstream ss;
	ss << "I am a triangle" << endl;
	ss << SceneObject::toString() << endl;
	ss << "V1: " << vertex1.x << ' ' << vertex1.y << ' ' << vertex1.z << endl;
	ss << "V2: " << vertex2.x << ' ' << vertex2.y << ' ' << vertex2.z << endl;
	ss << "V3: " << vertex3.x << ' ' << vertex3.y << ' ' << vertex3.z << endl;

	return ss.str();
}
