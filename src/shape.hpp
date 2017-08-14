#pragma once
class Material;
#include "cgra_math.hpp"
#include "ray.hpp"
#include "material.hpp"

class Shape
{
private:
	cgra::mat4 _objectToWorld;
	cgra::mat4 _worldToObject;
	Material* _material;

	virtual bool intersectP(Ray* ray) = 0;
public:
	Shape(cgra::mat4 objToWorld, cgra::mat4 worldToObj, Material* material);
	virtual ~Shape();

	cgra::mat4 GetWorldToObject();
	cgra::mat4 GetObjectToWorld();
	cgra::vec3 CalculateLight(Ray*);

	bool Intersect(Ray* ray);
	virtual int GetRadius() = 0;
};
