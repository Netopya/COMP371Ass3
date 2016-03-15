#include "Triangle.h"

Triangle::Triangle(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
	this->vertex3 = vertex3;
}
