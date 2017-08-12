#include "material.hpp"
#include "distantLight.hpp"
#include "renderer.hpp"

using namespace cgra;
using namespace std;

Material::Material(vec3 color, float reflectionFactor, float refractionFactor, float refractionIndex)
{
	_difuseColor = color;
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

vec4 Material::refract(vec4 direction, vec4 normal, float n2)
{

	float cosi = dot(direction, normal);
	float n1 = 1;	// Vacuum
	vec4 n = normal;
	if (cosi < 0)
	{
		cosi = -cosi;
	}
	else
	{
		swap(n1, n2);
		n = -normal;
	}

	float eta = n1 / n2;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	if(k<0)
	{
		// total internal reflection
		return vec4(0);
	}
	else
	{
		return eta * direction + (eta * cosi - sqrtf(k)) * n;
	}
}

vec3 Material::CalculateColor(Ray* ray, Shape* s)
{
	vec3 directLighting = vec3(0);
	vec3 reflectColor = vec3(0);
	vec3 refractColor = vec3(0);

	for (Light* light : Renderer::GetLights())
	{
		float dotProd;
		vec4 lightDirection = light->GetLightDirection(ray->GetHitPoint());

		vec4 hitNormal = ray->GetHitNormal();
		dotProd = dot(hitNormal, -lightDirection);

		if (dotProd > 0)
		{
			Ray newRay = Ray(ray->GetHitPoint(), -lightDirection, ray->GetCount() + 1);
			for (Shape* shape : Renderer::GetShapes())
			{
				if (shape != s && shape->Intersect(&newRay))
				{
					// Calculate direction
					auto v = newRay.GetHitPoint() - newRay.GetOrigin();
					auto v2 = normalize(v);
					auto v3 = v2 - newRay.GetDirection();

					// if obstruction lies between light source and point, do not consider this light source
					if (v3.x == 0 && v3.y == 0 && v3.z == 0 && v3.w == 0)
					{
						if(!newRay.AlongDirection())
						{
							std::cout << "ASDF";
						}
						dotProd = 0;
					}
				}
			}

			// Diffuse
			directLighting += _difuseColor * light->GetLightIntensity() * dotProd;

			//vec4 reflectionVector = reflect(lightDirection, hitNormal);
			//specular += _specColor * light->GetLightIntensity() * std::pow(std::max(0.f, dot(reflectionVector, -ray->GetDirection())), 32);
		}
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
		auto intRefractionDir = normalize(refract(ray->GetDirection(), ray->GetHitNormal(), _refractionIndex));
		auto refractRay = Ray(ray->GetHitPoint(), intRefractionDir, -1);
		s->Intersect(&refractRay);
		auto extRefractionDir = normalize(refract(intRefractionDir, refractRay.GetHitNormal2(), _refractionIndex));
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

	return directLighting + reflectColor + refractColor;
}

Material* CreateRedPlastic()
{
	return new Material(vec3(0.7, 0, 0), 0.7, 0, 0);
}

Material* CreateGlass()
{
	return new Material(vec3(0.01), 0.4, 0.8, 1.5);
}
