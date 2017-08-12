#pragma once

#include "cgra_math.hpp"

class Light
{
public:
	Light(cgra::vec3);

	virtual ~Light() = default;
	cgra::vec3 GetLightIntensity();

	virtual bool Obstructed(cgra::vec4) = 0;
	virtual cgra::vec4 GetLightDirection(cgra::vec4) = 0;
private:

	cgra::vec3 _lightIntensity;
};