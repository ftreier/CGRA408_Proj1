#include "material.hpp"
#include "distantLight.hpp"
#include "renderer.hpp"

using namespace cgra;
using namespace std;

Material::Material(vec3 doffColor, vec3 specColor, float reflectionFactor, float refractionFactor, float refractionIndex)
{
	_difuseColor = doffColor;
	_specColor = specColor;
	_reflectionFactor = reflectionFactor;
	_refractionFactor = refractionFactor;
	_refractionIndex = refractionIndex;
}

vec4 Material::reflect(const vec4 &d, const vec4 &n)
{
	vec4 dNormal = normalize(d);
	vec4 nNormal = normalize(n);
	return dNormal - 2 * dot(dNormal, nNormal) * nNormal;
}

vec4 Material::refract(vec4 direction, vec4 normal, float n1, float n2)
{
	float cosi = dot(direction, normal);
	vec4 n = normal;

	float eta = n1 / n2;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	if(k < 0)
	{
		// total internal reflection
		return vec4(0);
	}
	
	return eta * direction + (eta * cosi - sqrtf(k)) * n;
}

vec3 Material::CalculateColor(Ray* ray, Shape* s)
{
	vec3 directLighting = vec3(0);
	vec3 specular;
	vec3 reflectColor = vec3(0);
	vec3 refractColor = vec3(0);

	for (Light* light : Renderer::GetLights())
	{
		//ray->ConvertToSpace(light->GetWorldToObject());
		float dotProd;
		vec4 lightDirection = light->GetLightDirection(ray->GetHitPoint());

		vec4 hitNormal = ray->GetHitNormal();
		dotProd = dot(hitNormal, -lightDirection);

		if (dotProd > 0)
		{
			Ray newRay = Ray(ray->GetHitPoint(), -lightDirection, ray->GetCount() + 1);
			for (Shape* shape : Renderer::GetShapes())
			{
				if (shape != s && shape->Intersect(&newRay) && newRay.AlongDirection())
				{
					dotProd = 0;
					break;
				}
			}

			// Diffuse
			directLighting += _difuseColor * light->GetLightIntensity() * dotProd;

			// Specular
			vec4 reflectionVector = reflect(lightDirection, hitNormal);
			specular += _specColor * light->GetLightIntensity() * std::pow(std::max(0.f, dot(reflectionVector, -ray->GetDirection())), 32);
		}

		//ray->ConvertToSpace(light->GetObjectToWorld());
	}

	if (_reflectionFactor > 0 && ray->GetCount() < Renderer::MAX_RAYCAST_DEPTH)
	{
		vec4 reflectionVector = reflect(ray->GetDirection(), ray->GetHitNormal());
		Ray newRay = Ray(ray->GetHitPoint(), reflectionVector, ray->GetCount() + 1);
		float minDist = numeric_limits<float>::max();
		Shape* closestShape = nullptr;

		for (Shape* shape : Renderer::GetShapes())
		{
			if (shape->Intersect(&newRay))
			{
				if(newRay.GetDistance() < minDist)
				{
					closestShape = shape;
					minDist = newRay.GetDistance();
				}
			}
		}

		if(closestShape != nullptr && closestShape != s)
		{
			closestShape->Intersect(&newRay);
			reflectColor = closestShape->CalculateLight(&newRay) * _reflectionFactor;
		}
	}

	if (_refractionFactor > 0 && ray->GetCount() < Renderer::MAX_RAYCAST_DEPTH)
	{
		auto intRefractionDir = normalize(refract(ray->GetDirection(), ray->GetHitNormal(), 1, _refractionIndex));
		auto refractRay = Ray(ray->GetHitPoint(), intRefractionDir, -1);
		if(!s->Intersect(&refractRay))
		{
			std::cout << "ASDF";
		}
		auto extRefractionDir = normalize(refract(intRefractionDir, -refractRay.GetHitNormal2(), _refractionIndex, 1));
		auto newRay = Ray(refractRay.GetHitPoint2(), extRefractionDir, ray->GetCount() + 1);

		float minDist = numeric_limits<float>::max();
		Shape* closestShape = nullptr;

		for (Shape* shape : Renderer::GetShapes())
		{
			if (shape->Intersect(&newRay))
			{
				if (newRay.GetDistance() < minDist)
				{
					closestShape = shape;
					minDist = newRay.GetDistance();
				}
			}
		}

		if (closestShape != nullptr && closestShape != s)
		{
			closestShape->Intersect(&newRay);
			refractColor = closestShape->CalculateLight(&newRay) * _refractionFactor;
		}
	}

	return directLighting + specular + reflectColor + refractColor;
}

Material* CreateRedPlastic()
{
	return new Material(vec3(0.7, 0, 0), vec3(0.7, 0.5, 0.5), 0.7, 0, 0);
}

Material* CreateGlass()
{
	return new Material(vec3(0.01), vec3(0.5), 0.0, 1, 1.5);
}
