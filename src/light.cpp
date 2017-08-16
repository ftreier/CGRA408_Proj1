#include "light.hpp"

using namespace cgra;

Light::Light(cgra::vec3 lightIntensity, mat4 objectToWorld, int samples, float disperisonFactor)
{
	_lightIntensity = lightIntensity;
	_objectToWorld = objectToWorld;
	_worldToObject = inverse(objectToWorld);
	_numberOfSamples = samples;
	_dispersionFactor = disperisonFactor;
}

cgra::vec3 Light::GetLightIntensity()
{
	return _lightIntensity;
}

cgra::mat4 Light::GetWorldToObject()
{
	return _worldToObject;
}

cgra::mat4 Light::GetObjectToWorld()
{
	return _objectToWorld;
}

int Light::GetNumberOfSamples()
{
	return _numberOfSamples;
}

float Light::GetDispersionFactor()
{
	return _dispersionFactor;
}
