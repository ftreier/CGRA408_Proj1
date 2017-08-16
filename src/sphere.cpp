#include "sphere.hpp"

using namespace cgra;

Sphere::Sphere(float radius, mat4 objToWorld, mat4 worldToObj, Material* mat)
	: Shape(objToWorld, worldToObj, mat)
{
	_radius = radius;
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

	float A = ray->GetDirection().x*ray->GetDirection().x + ray->GetDirection().y*ray->GetDirection().y + ray->GetDirection().z*ray->GetDirection().z;
	float B = 2 * (ray->GetDirection().x*ray->GetOrigin().x + ray->GetDirection().y*ray->GetOrigin().y + ray->GetDirection().z*ray->GetOrigin().z);
	float C = ray->GetOrigin().x*ray->GetOrigin().x + ray->GetOrigin().y*ray->GetOrigin().y + ray->GetOrigin().z*ray->GetOrigin().z - _radius*_radius;

	float discriminant = B * B - 4.0f * A * C;
	if (discriminant < 0.0f)
	{
		return false;
	}

	float rootDiscrim = sqrtf(discriminant);

	float q;
	if (B < 0)
	{
		q = -.5f * (B - rootDiscrim);
	}
	else
	{
		q = -.5f * (B + rootDiscrim);
	}

	float t0 = q / A;
	float t1 = C / q;
	if (t0 > t1)
	{
		std::swap(t0, t1);
	}

	SetHitPoint(ray, t0, t1);
	return true;
}

Sphere *CreateSphere(float radius, vec3 origin, Material* mat)
{
	mat4 m = mat4::translate(origin);
	return new Sphere(radius, m, inverse(m), mat);
}