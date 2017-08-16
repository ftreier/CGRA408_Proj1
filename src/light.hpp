#pragma once

#include "cgra_math.hpp"

class Light
{
public:
	Light(cgra::vec3, cgra::mat4 objectToWorld, int samples, float disperisonFactor);

	virtual ~Light() = default;
	cgra::vec3 GetLightIntensity();

	virtual bool Obstructed(cgra::vec4) = 0;
	virtual cgra::vec4 GetLightDirection(cgra::vec4) = 0;
	cgra::mat4 GetWorldToObject();
	cgra::mat4 GetObjectToWorld();
	int GetNumberOfSamples();
	float GetDispersionFactor();
private:

	cgra::vec3 _lightIntensity;
	cgra::mat4 _objectToWorld;
	cgra::mat4 _worldToObject;
	int _numberOfSamples;
	float _dispersionFactor;
};