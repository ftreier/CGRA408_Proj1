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

void Sphere::SetHitPoint(Ray* ray, float t0, float t1)
{
	ray->SetHitPoint1(ray->GetOrigin() + t0 * ray->GetDirection());
	auto d = ray->GetHitPoint();
	d.w = 0;
	ray->SetHitnormal1(normalize(d));

	ray->SetHitPoint2(ray->GetOrigin() + t1 * ray->GetDirection());
	d = ray->GetHitPoint2();
	d.w = 0;
	ray->SetHitnormal2(normalize(d));
}

bool Sphere::intersectP(Ray* ray)
{
	float phi;

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

	//vec4 hit = ray->GetOrigin() + t0 * ray->GetDirection();
	SetHitPoint(ray, t0, t1);
	return true;
}

Sphere *CreateSphere(float radius, vec3 origin, Material* mat)
{
	// transform to soace
	mat4 m = mat4::translate(origin);
	return new Sphere(radius, m, inverse(m), mat);
}