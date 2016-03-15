#pragma once

#include "SceneObject.h"
#include "../glm/glm.hpp"

class Triangle : public SceneObject {
private:
	glm::vec3 position;
	float radius;
public:
};