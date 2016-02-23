#pragma once
#include "stdafx.h"
#include <glm.hpp>
class Sun
{
public:
	Sun();
	~Sun();
	void CreateSun();

private:
	glm::vec3 shineDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 sunPosition = glm::vec3(1.0f, 1.0f, 1.0f);
};

