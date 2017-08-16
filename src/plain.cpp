#include "plain.hpp"

using namespace cgra;

Plain::Plain(float width, float height, mat4 objToWorld, mat4 worldToObj, Material* mat)
	: Shape(objToWorld, worldToObj, mat)
{
	_width = width;
	_height = height;
}

void Plain::SetHitPoint(Ray* ray, vec4 hitPt)
{
	ray->SetHitPoint1(hitPt);
	ray->SetHitPoint2(hitPt);
	ray->SetHitnormal1(vec4(0, 0, -1, 0));
	ray->SetHitnormal2(vec4(0, 0, -1, 0));
}

bool Plain::intersectP(Ray* ray)
{
	if (fabsf(ray->GetDirection().z) < 1e-7)
	{
		return false;
	}

	float thit = -ray->GetOrigin().z / ray->GetDirection().z;
	vec4 hit = ray->GetOrigin() + thit * ray->GetDirection();

	if(abs(hit.x) > _width / 2.0 || abs(hit.y) > _height / 2.0)
	{
		return false;
	}

	SetHitPoint(ray, hit);

	return true;
}

Plain* CreatePlain(float width, float height, vec3 origin, vec3 rotation, Material* mat)
{
	//mat4 m = mat4::rotateY(rotation.y);
	mat4 m = mat4::rotateX(rotation.x) * mat4::rotateX(rotation.x) * mat4::rotateY(rotation.y) * mat4::rotateZ(rotation.z);
	m = mat4::translate(origin) * m;
	return new Plain(width, height, m, inverse(m), mat);
}
