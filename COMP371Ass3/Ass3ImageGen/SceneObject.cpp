#include "SceneObject.h"
#include <sstream>

SceneObject::SceneObject(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

string SceneObject::toString()
{
	std::stringstream ss;
	ss << "I am a SceneObject" << endl;
	ss << "Ambient: " << ambient.x << ' ' << ambient.y << ' ' << ambient.z << endl;
	ss << "Diffuse: " << diffuse.x << ' ' << diffuse.y << ' ' << diffuse.z << endl;
	ss << "Specular: " << specular.x << ' ' << specular.y << ' ' << specular.z << endl;
	ss << "Sphiniess: " << shininess << endl;

	return ss.str();
}
