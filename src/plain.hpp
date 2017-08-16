#pragma once
#include "shape.hpp"

class Plain: public Shape
{
private:
	float _width;
	float _height;

	bool intersectP(Ray* ray) override;
public:
	Plain(float width, float height, cgra::mat4 objToWorld, cgra::mat4 worldToObj, Material* mat);
	void SetHitPoint(Ray* ray, cgra::vec4 hitPt);
};

Plain* CreatePlain(float width, float height, cgra::vec3 origin, cgra::vec3 rotation, Material* mat);
