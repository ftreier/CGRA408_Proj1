#include "distantLight.hpp"
#include "renderer.hpp"

using namespace cgra;

DistantLight::DistantLight(vec4 direction, vec3 lightIntensity, mat4 objectToWorld, int numberOfSamples, float disperisonFactor)
	: Light(lightIntensity, objectToWorld, numberOfSamples, disperisonFactor)
{
	_direction = normalize(objectToWorld * direction);
}

bool DistantLight::Obstructed(vec4 pt)
{
	Ray r = Ray(pt, _direction * -1, 0);
	//for (Shape shape : Renderer::GetShapes())
	//{
	//	if(shape.Intersect(r))
	//	{
	//		return true;
	//	}
	//}

	return false;
}

cgra::vec4 DistantLight::GetLightDirection(cgra::vec4)
{
	return _direction;
}

DistantLight* CreateDistantLight(cgra::vec4 direction, cgra::vec3 lightIntensity, int numberOfSamples, float disperisonFactor)
{
	return new DistantLight(direction, lightIntensity, mat4::translate(0, 0, 0), numberOfSamples, disperisonFactor);
}
