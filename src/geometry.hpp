//---------------------------------------------------------------------------
//
// Copyright (c) 2016 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "cgra_math.hpp"
//#include "opengl.hpp"


struct vertex {
	int p = 0; // index for point in m_points
	int t = 0; // index for uv in m_uvs
	int n = 0; // index for normal in m_normals
};

struct triangle {
	vertex v[3]; //requires 3 verticies
};

struct BoundingBox
{
	float minx = std::numeric_limits<float>::max();
	float miny = std::numeric_limits<float>::max();
	float minz = std::numeric_limits<float>::max();
	float maxx = -std::numeric_limits<float>::max();
	float maxy = -std::numeric_limits<float>::max();
	float maxz = -std::numeric_limits<float>::max();
};

class Geometry {
private:

	BoundingBox _bb;

	// Feilds for storing raw obj information
	std::string m_filename;

	bool m_wireFrameOn = false;

	// IDs for the display list to render
	//GLuint m_displayListPoly = 0; // DisplayList for Polygon
	//GLuint m_displayListWire = 0; // DisplayList for Wireframe

	void readOBJ(std::string);

	void createNormals();

	void createDisplayListPoly();
	void createDisplayListWire();

public:
	Geometry(std::string);
	~Geometry();

	void renderGeometry();
	void toggleWireFrame();
	void createBoundingBox();
	BoundingBox GetBoundingBox();

	std::vector<triangle> m_triangles;	// Triangle/Face list
	std::vector<cgra::vec3> m_points;	// Point list
	std::vector<cgra::vec2> m_uvs;		// Texture Coordinate list
	std::vector<cgra::vec3> m_normals;	// Normal list
};