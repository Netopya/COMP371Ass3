#include "Sphere.h"

#include <sstream>
#include <math.h>

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

float Sphere::vecHit(glm::vec3 position, glm::vec3 vector)
{
	float b = 2.0f * (vector.x * (position.x - this->position.x) + vector.y * (position.y - this->position.y) + vector.z * (position.z - this->position.z));
	float c = pow(position.x - this->position.x, 2) + pow(position.y - this->position.y, 2) + pow(position.z - this->position.z, 2) - pow(radius, 2);

	float descriminant = pow(b,2) - 4.0f * c;
	if (descriminant < 0)
	{
		return -1;
	}

	descriminant = sqrt(descriminant);

	float t1 = (-1.0f * b + descriminant) / 2;
	float t2 = (-1.0f * b - descriminant) / 2;


	return t1 < t2 ? t1 : t2;
}

glm::vec3 Sphere::getNormalAtPoint(glm::vec3 point)
{
	return glm::normalize(point - position);
}
