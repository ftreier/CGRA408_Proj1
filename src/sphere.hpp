#pragma once

#include "shape.hpp"

class Sphere : public Shape
{
private:
	float _radius;

	void SetHitPoint(Ray* ray, float t0, float t1);
	bool intersectP(Ray* ray) override;
public:
	Sphere(float radius, cgra::mat4 objToWorld, cgra::mat4 worldToObj, Material* mat);
	int GetRadius() override;
};

Sphere *CreateSphere(float radius, cgra::vec3 origin, Material* mat);