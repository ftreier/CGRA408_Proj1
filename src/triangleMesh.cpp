#include "triangleMesh.hpp"

using namespace cgra;
using namespace std;

TriangleMesh::TriangleMesh(mat4& objToWorld, mat4& worldToObj, Material* material, string filename)
	: Shape(objToWorld, worldToObj, material)
{
	_mesh = new Geometry(filename);
	_mesh->createBoundingBox();
}

TriangleMesh::~TriangleMesh()
{
	delete _mesh;
}

bool TriangleMesh::isInsideBoundingBox(Ray* ray)
{
	auto box = _mesh->GetBoundingBox();

	auto origin = ray->GetOrigin();
	auto direction = normalize(ray->GetDirection());

	float txmin = (box.minx - origin.x) / direction.x;
	float txmax = (box.maxx - origin.x) / direction.x;
	if (txmin > txmax)
	{
		swap(txmin, txmax);
	}

	float tymin = (box.miny - origin.y) / direction.y;
	float tymax = (box.maxy - origin.y) / direction.y;
	if (tymin > tymax)
	{
		swap(tymin, tymax);
	}

	float tzmin = (box.minz - origin.z) / direction.z;
	float tzmax = (box.maxz - origin.z) / direction.z;
	if (tzmin > tzmax)
	{
		swap(tzmin, tzmax);
	}

	if ((txmin > tymax) || (tymin > txmax))
	{
		return false;
	}

	float tmin = min(txmin, tymin);
	float tmax = max(txmax, tymax);

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	float minHitDistance = min(tmin, tzmin);

	ray->SetHitPoint1(vec4(origin.x + minHitDistance, origin.y, origin.z, 1));

	return true;
}

TriangleMesh* CreateTriangleMesh(string filename, vec3 origin, vec3 rotation, Material* mat)
{
	auto m = mat4::rotateX(rotation.x) * mat4::rotateX(rotation.x) * mat4::rotateY(rotation.y) * mat4::rotateZ(rotation.z);
	m = mat4::translate(origin) * m;
	auto inv = inverse(m);
	return new TriangleMesh(m, inv, mat, filename);
}

bool TriangleMesh::getTriangleIntersection(Ray* ray)
{
	//for (triangle t : _mesh->m_triangles)
	//{
	//	// test for intersection with triangle
	//	// If intersect:
	//	// return true;
	//}
	
	return false;
}

bool TriangleMesh::intersectP(Ray* ray)
{
	if(isInsideBoundingBox(ray))
	{
		getTriangleIntersection(ray);
		return true;
	}
	return false;
}