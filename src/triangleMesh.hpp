#pragma once
#include "shape.hpp"
#include "geometry.hpp"

class TriangleMesh: public Shape
{
public:
	TriangleMesh(cgra::mat4& objToWorld,
		cgra::mat4& worldToObj,
		Material* material,
	    std::string filename);
	~TriangleMesh();
private:
	Geometry* _mesh;
	bool getTriangleIntersection(Ray* ray);
	bool intersectP(Ray* ray) override;

	bool isInsideBoundingBox(Ray* ray);
};

TriangleMesh* CreateTriangleMesh(std::string, cgra::vec3, cgra::vec3, Material*);