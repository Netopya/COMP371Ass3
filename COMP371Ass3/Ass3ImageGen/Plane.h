#pragma once

#include "SceneObject.h"
#include "../glm/glm.hpp"

class Plane : public SceneObject{
private:
	glm::vec3 normal;
	glm::vec3 position;
};
