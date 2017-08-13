#include "light.hpp"

using namespace cgra;

Light::Light(cgra::vec3 lightIntensity, mat4 objectToWorld)
{
	_lightIntensity = lightIntensity;
	_objectToWorld = objectToWorld;
	_worldToObject = inverse(objectToWorld);
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
