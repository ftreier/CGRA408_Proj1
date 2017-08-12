#include "sphere.hpp"

using namespace cgra;

Sphere::Sphere(float radius, mat4 objToWorld, mat4 worldToObj, Material* mat)
	: Shape(objToWorld, worldToObj, mat)
{
	_radius = radius;
}

bool Quadratic(float A, float B, float C, float *t0, float *t1) {
	// Find quadratic discriminant
	float discriminant = B * B - 4.f * A * C;
	if (discriminant < 0.)
	{
		return false;
	}

	float rootDiscrim = sqrtf(discriminant);

	// Compute quadratic _t_ values
	float q;
	if (B < 0)
	{
		q = -.5f * (B - rootDiscrim);
	}
	else
	{
		q = -.5f * (B + rootDiscrim);
	}

	*t0 = q / A;
	*t1 = C / q;
	if (*t0 > *t1)
	{
		std::swap(*t0, *t1);
	}
	return true;
}

bool Sphere::intersectP(Ray* ray)
{
	float phi;
	//Point phit;
	//// Transform _Ray_ to object space
	//Ray ray;
	//ray->ConvertToSpace(GetWorldToObject());
	//(*WorldToObject)(r, &ray);

	//// Compute quadratic sphere coefficients
	float A = ray->GetDirection().x*ray->GetDirection().x + ray->GetDirection().y*ray->GetDirection().y + ray->GetDirection().z*ray->GetDirection().z;
	float B = 2 * (ray->GetDirection().x*ray->GetOrigin().x + ray->GetDirection().y*ray->GetOrigin().y + ray->GetDirection().z*ray->GetOrigin().z);
	float C = ray->GetOrigin().x*ray->GetOrigin().x + ray->GetOrigin().y*ray->GetOrigin().y + ray->GetOrigin().z*ray->GetOrigin().z - _radius*_radius;

	//// Solve quadratic equation for _t_ values
	float t0, t1;
	if (!Quadratic(A, B, C, &t0, &t1))
	{
		return false;
	}

	//float x = ray->GetOrigin().x + t0 * ray->GetDirection().x;
	//float y = ray->GetOrigin().y + t0 * ray->GetDirection().y;
	//float z = ray->GetOrigin().z + t0 * ray->GetDirection().z;

	vec4 hit = ray->GetOrigin() + t0 * ray->GetDirection();
	ray->SetHitPoint(t0, t1);
	//if(x != hit.x || y != hit.y || z != hit.z || hit.w != 1)
	//{
	//	std::cout << "ASDF";
	//}

	//if(x > _radius || y > _radius || z > _radius || sqrt(x*x+y*y+z*z) - 0.001 > _radius)
	//{
	//	std::cout << "asdf";
	//}
	//ray->ConvertToSpace(GetObjectToWorld());
	return true;
}

Sphere *CreateSphere(float radius, vec3 origin, Material* mat)
{
	// transform to soace
	mat4 m = mat4::translate(origin);
	return new Sphere(radius, m, inverse(m), mat);
}