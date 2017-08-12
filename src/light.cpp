#include "light.hpp"

Light::Light(cgra::vec3 lightIntensity)
{
	_lightIntensity = lightIntensity;
}

cgra::vec3 Light::GetLightIntensity()
{
	return _lightIntensity;
}
