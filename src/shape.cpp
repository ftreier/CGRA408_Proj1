#include "shape.hpp"
#include "renderer.hpp"
#include "distantLight.hpp"

using namespace cgra;
using namespace std;

Shape::Shape(mat4 objToWorld, mat4 worldToObj, Material* material)
{
	_objectToWorld = objToWorld;
	_worldToObject = worldToObj;
	_material = material;
}

Shape::~Shape()
{
	//free(_objectToWorld);
	//free(_worldToObject);
}

mat4 Shape::GetWorldToObject()
{
	return _worldToObject;
}

cgra::mat4 Shape::GetObjectToWorld()
{
	return _objectToWorld;
}

cgra::vec3 Shape::CalculateLight(Ray* ray)
{
	return _material->CalculateColor(ray, this);
}

bool Shape::Intersect(Ray* ray)
{
	ray->ConvertToSpace(GetWorldToObject());
	bool b = intersectP(ray);
	ray->ConvertToSpace(GetObjectToWorld());
	return b;
}

