#include "Triangle.h"


#include <sstream>

Triangle::Triangle(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3) : SceneObject(ambient, diffuse, specular, shininess)
{
	this->vertex1 = vertex1;
	this->vertex2 = vertex2;
	this->vertex3 = vertex3;
	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
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

float Triangle::vecHit(glm::vec3 position, glm::vec3 vector)
{
	// Find the location of the hit on the plane formed by the vector

	float dom = glm::dot(normal, vector);

	if (dom == 0)
	{
		return -1.0f;
	}

	float t = (glm::dot(normal, this->vertex1) - glm::dot(normal, position)) / dom;

	glm::vec3 placeOnTriangle = position + t * vector;

	
	// Determine if the point is on the triangle
	// Based on http://stackoverflow.com/questions/2049582/how-to-determine-a-point-in-a-2d-triangle
	bool b1, b2, b3;

	b1 = sign(placeOnTriangle, vertex1, vertex2) < 0.0f;
	b2 = sign(placeOnTriangle, vertex2, vertex3) < 0.0f;
	b3 = sign(placeOnTriangle, vertex3, vertex1) < 0.0f;

	if ((b1 == b2) && (b2 == b3))
	{
		return t;
	}
	else
	{
		return -1.0f;
	}	
}

glm::vec3 Triangle::getNormalAtPoint(glm::vec3 point)
{
	return normal;
}

float Triangle::sign(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	// Helper function from http://stackoverflow.com/questions/2049582/how-to-determine-a-point-in-a-2d-triangle
	return glm::dot((p1.x - p3.x), (p2.y - p3.y)) - glm::dot((p2.x - p3.x),(p1.y - p3.y));
}