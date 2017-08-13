#pragma once
#include "light.hpp"
#include "cgra_math.hpp"

class DistantLight : public Light
{
public:
	DistantLight(cgra::vec4 direction, cgra::vec3 lightIntensity, cgra::mat4 objectToWorld);
	bool Obstructed(cgra::vec4) override;
	cgra::vec4 GetLightDirection(cgra::vec4) override;

private:
	cgra::vec4 _direction;
};

DistantLight* CreateDistantLight(cgra::vec4, cgra::vec3);