#include "ray.hpp"

using namespace cgra;

Ray::Ray(vec4 origin, vec4 direction, int hitCount)
{
	_origin = origin;
	_direction = direction;
	_hitCount = hitCount;
}

vec4 Ray::GetOrigin()
{
	return _origin;
}

vec4 Ray::GetDirection()
{
	return _direction;
}

int Ray::GetCount()
{
	return _hitCount;
}

void Ray::ConvertToSpace(mat4 m)
{


	if (length(_origin) != 0)
	{
		_origin = m * GetOrigin();
	}

	if (length(_direction) != 0)
	{
		_direction = normalize(m * GetDirection());
	}

	if (length(_hitPoint) != 0)
	{
		_hitPoint = m * _hitPoint;
	}

	if (length(_hitNormal) != 0)
	{
		_hitNormal = normalize(m * _hitNormal);
	}

	if (length(_hitPoint2) != 0)
	{
		_hitPoint2 = m * _hitPoint2;
	}

	if (length(_hitNormal2) != 0)
	{
		_hitNormal2 = normalize(m * _hitNormal2);
	}
}

//void Ray::SetHitPoint(float pt1, float pt2)
//{
//	_hitPoint = _hitNormal = _origin + pt1 * _direction;
//	_hitNormal.w = 0;
//
//	if (length(_hitNormal) != 0)
//	{
//		_hitNormal = normalize(_hitNormal);
//	}
//
//	_hitPoint2 = _hitNormal2 = _origin + pt2 * _direction;
//	_hitNormal2.w = 0;
//
//	if (length(_hitNormal2) != 0)
//	{
//		_hitNormal2 = normalize(_hitNormal2);
//	}
//
//	_hitDistance = length(_origin - _hitPoint);
//}

cgra::vec4 Ray::GetHitNormal()
{
	return _hitNormal;
}

cgra::vec4 Ray::GetHitPoint()
{
	return _hitPoint;
}

cgra::vec4 Ray::GetHitPoint2()
{
	return _hitPoint2;
}

float Ray::GetDistance()
{
	return length(_origin - _hitPoint);
}

bool Ray::AlongDirection()
{
	auto v = normalize(GetHitPoint() - GetOrigin()) - (GetDirection());

	// if obstruction lies between light source and point, do not consider this light source
	return v.x <= 0.0001 && v.y <= 0.0001 && v.z <= 0.0001 && v.w <= 0.0001;
}

cgra::vec4 Ray::GetHitNormal2()
{
	return _hitNormal2;
}

void Ray::SetHitPoint1(cgra::vec4 point)
{
	_hitPoint = point;
}

void Ray::SetHitnormal1(cgra::vec4 normal)
{
	_hitNormal = normal;
}

void Ray::SetHitPoint2(cgra::vec4 point)
{
	_hitPoint2 = point;
}

void Ray::SetHitnormal2(cgra::vec4 normal)
{
	_hitNormal2 = normal;
}
