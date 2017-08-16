#include "material.hpp"
#include "distantLight.hpp"
#include "renderer.hpp"

using namespace cgra;
using namespace std;
using namespace math;

Material::Material(vec3 doffColor, vec3 specColor, float reflectionFactor, float refractionFactor, float refractionIndex)
{
	_difuseColor = doffColor;
	_specColor = specColor;
	_reflectionFactor = reflectionFactor;
	_refractionFactor = refractionFactor;
	_refractionIndex = refractionIndex;

	random_device randomDevice;
	_randomGenerator = mt19937(randomDevice());
	_randomDistribution = uniform_real_distribution<float>(-0.1f, 0.1f);
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

	float eta = n1 / n2;
	float k = 1.0f - eta * eta * (1.0f - cosi * cosi);
	if(k < 0)
	{
		// total internal reflection
		return vec4(0.0f);
	}
	
	return eta * direction + (eta * cosi - sqrt(k)) * normal;
}

vec3 Material::CalculateColor(Ray* ray, Shape* s)
{
	vec3 directLighting = vec3(0.0f);
	vec3 specular;
	vec3 reflectColor = vec3(0.0f);
	vec3 refractColor = vec3(0.0f);

	for (Light* light : Renderer::GetLights())
	{
		//ray->ConvertToSpace(light->GetWorldToObject());
		float dotProd;
		vec4 lightDirection = light->GetLightDirection(ray->GetHitPoint());

		vec4 hitNormal = ray->GetHitNormal();
		dotProd = dot(hitNormal, -lightDirection);

		if (dotProd > 0)
		{
			auto dl = vec3(0.0f);
			auto sp = vec3(0.0f);
			int count = light->GetNumberOfSamples();
			for (int i = 0; i < count; i++)
			{
				vec4 offset = vec4(0.0f);
				if (i > 1)
				{
					offset = vec4(rn(), rn(), rn(), 0.0f) * light->GetDispersionFactor();
				}

				auto dp = dotProd;
				Ray newRay = Ray(ray->GetHitPoint(), -(lightDirection + offset), ray->GetCount() + 1);
				for (Shape* shape : Renderer::GetShapes())
				{
					if (shape != s && shape->Intersect(&newRay) && newRay.AlongDirection())
					{
						dp = 0.0f;
						break;
					}
				}

				// Diffuse
				dl += _difuseColor * light->GetLightIntensity() * dp;

				// Specular
				if (dp > 0)
				{
					vec4 reflectionVector = reflect(lightDirection, hitNormal);
					sp += _specColor * light->GetLightIntensity() * std::pow(std::max(0.f, dot(reflectionVector, -ray->GetDirection())), 32);
				}
			}
			directLighting += dl / static_cast<float>(count);
			specular += sp / static_cast<float>(count);
		}

		//ray->ConvertToSpace(light->GetObjectToWorld());
	}

	if (_reflectionFactor > 0 && ray->GetCount() < Renderer::GetRayCasteDepth())
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

	if (_refractionFactor > 0 && ray->GetCount() < Renderer::GetRayCasteDepth())
	{
		auto intRefractionDir = normalize(refract(ray->GetDirection(), ray->GetHitNormal(), 1, _refractionIndex));
		auto refractRay = Ray(ray->GetHitPoint(), intRefractionDir, -1);
		if (s->Intersect(&refractRay))
		{
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
	}

	return directLighting + specular + reflectColor + refractColor;
}

float Material::rn()
{
	return _randomDistribution(_randomGenerator);
}

Material* CreateRedPlastic()
{
	return new Material(vec3(0.7f, 0.0f, 0.0f), vec3(0.7f, 0.5f, 0.5f), 0.7f, 0.0f, 0.0f);
}

Material* CreateGlass()
{
	return new Material(vec3(0.01f), vec3(0.5f), 0.0f, 1.0f, 1.5f);
}
