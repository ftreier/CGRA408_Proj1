#pragma once
#include "cgra_math.hpp"

class Ray
{
	cgra::vec4 _origin;
	cgra::vec4 _direction;
	int _hitCount;
	cgra::vec4 _hitPoint;
	cgra::vec4 _hitPoint2;
	cgra::vec4 _hitNormal;
	cgra::vec4 _hitNormal2;
	float _hitDistance;
	
public:
	Ray(cgra::vec4 origin, cgra::vec4 direction, int hitCount);
	cgra::vec4 GetOrigin();
	cgra::vec4 GetDirection();
	int GetCount();
	void ConvertToSpace(cgra::mat4);
	void SetHitPoint(float pt1, float pt2);
	cgra::vec4 GetHitNormal();
	cgra::vec4 GetHitPoint();
	cgra::vec4 GetHitPoint2();
	float GetDistance();
	bool AlongDirection();
	cgra::vec4 GetHitNormal2();
};
