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

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "cgra_math.hpp"
#include "geometry.hpp"
#include <cctype>

using namespace std;
using namespace cgra;


Geometry::Geometry(string filename) {
	m_filename = filename;
	readOBJ(filename);
	//if (m_triangles.size() > 0) {
	//	createDisplayListPoly();
	//	createDisplayListWire();
	//}
}


Geometry::~Geometry() { }


void Geometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));

	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;
	
	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {

				vector<vertex> verts;
				while (objLine.good()){
					vertex v;

					//-------------------------------------------------------------
					// [Assignment 1] :
					// Modify the following to parse the bunny.obj. It has no uv
					// coordinates so each vertex for each face is in the format
					// v//vn instead of the usual v/vt/vn.
					//
					// Modify the following to parse the dragon.obj. It has no
					// normals or uv coordinates so the format for each vertex is
					// v instead of v/vt/vn or v//vn.
					//
					// Hint : Check if there is more than one uv or normal in
					// the uv or normal vector and then parse appropriately.
					//-------------------------------------------------------------

					objLine >> v.p;		// Scan in position index
					auto nextChar = objLine.peek();
					if (nextChar == '/') // obj file allow for just one vertex (dragon.obj)
					{
						objLine.ignore(1);	// Ignore the '/' character
						nextChar = objLine.peek(); 
						if (nextChar != '/') // no texture coordinate (bunny.obj)
						{
							objLine >> v.t;		// Scan in uv (texture coord) index
						}

						nextChar = objLine.peek();
						if (nextChar == '/') // vertexes in the format 'v/vt'
						{
							objLine.ignore(1);	// Ignore the '/' character
							objLine >> v.n;		// Scan in normal index
						}
					}
					else
					{
						v.n = v.p;
						//v.n = m_triangles.size();
						//v.n = m_triangles.size() * 3 + verts.size();
					}

					verts.push_back(v);
				}

				// IF we have 3 verticies, construct a triangle
				if(verts.size() >= 3)
				{
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);
				}
			}
		}
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;

	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();
}

void Geometry::createNormals()
{
	m_normals = vector<vec3>(m_points.size());
	for (vector<vector3<float>>::size_type i = 0; i < m_triangles.size(); i++)
	{
		auto tri = m_triangles[i];
		auto a = m_points[tri.v[0].p];
		auto b = m_points[tri.v[1].p];
		auto c = m_points[tri.v[2].p];
		vec3 normal = cgra::cross(b - a, c - a);
		
		m_normals[tri.v[0].n] += normal;
		m_normals[tri.v[1].n] += normal;
		m_normals[tri.v[2].n] += normal;
	}

	for (vector<vector3<float>>::size_type i = 1; i < m_normals.size(); i++)
	{
		try
		{
			normalize(m_normals[i]);
		}
		catch (exception ex)
		{
			cout << "Could not normalize vector " << m_normals[i] << "at position " << i;
		}
	}
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as wireframe model
//-------------------------------------------------------------
void Geometry::createDisplayListPoly()
{
	//// Delete old list if there is one
	//if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	//// Create a new list
	//cout << "Creating Poly Geometry" << endl;
	//m_displayListPoly = glGenLists(1);
	//glNewList(m_displayListPoly, GL_COMPILE);

	//glBegin(GL_TRIANGLES);

	//for (vector<triangle>::size_type i = 0; i < m_triangles.size(); i++)
	//{
	//	for(int j = 0; j < 3; j++)
	//	{
	//		auto p = m_points[m_triangles[i].v[j].p];
	//		auto n = m_normals[m_triangles[i].v[j].n];
	//		glNormal3f(n.x, n.y, n.z);
	//		glVertex3f(p.x, p.y, p.z);
	//	}
	//}

	//glEnd();
	//glEndList();
	//cout << "Finished creating Poly Geometry" << endl;
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as polygon model
//-------------------------------------------------------------
void Geometry::createDisplayListWire()
{
	//// Delete old list if there is one
	//if (m_displayListWire) glDeleteLists(m_displayListWire, 1);

	//// Create a new list
	//cout << "Creating Wire Geometry" << endl;
	//m_displayListWire = glGenLists(1);
	//glNewList(m_displayListWire, GL_COMPILE);

	//for (vector<triangle>::size_type i = 0; i < m_triangles.size(); i++)
	//{
	//	glBegin(GL_LINE_LOOP);

	//	for (auto j = 0; j < 3; j++)
	//	{
	//		auto p = m_points[m_triangles[i].v[j].p];
	//		auto n = m_normals[m_triangles[i].v[j].n];
	//		glNormal3f(n.x, n.y, n.z);
	//		glVertex3f(p.x, p.y, p.z);
	//	}

	//	glEnd();
	//}

	//glEndList();
	//cout << "Finished creating Wire Geometry" << endl;
}


void Geometry::renderGeometry() {
	//if (m_wireFrameOn) {

	//	//-------------------------------------------------------------
	//	// [Assignment 1] :
	//	// When moving on to displaying your obj, comment out the
	//	// wire_cow function & uncomment the glCallList function
	//	//-------------------------------------------------------------

	//	glShadeModel(GL_SMOOTH);
	//	//wire_cow();
	//	glCallList(m_displayListWire);

	//} else {

	//	//-------------------------------------------------------------
	//	// [Assignment 1] :
	//	// When moving on to displaying your obj, comment out the
	//	// cow function & uncomment the glCallList function
	//	//-------------------------------------------------------------

	//	glShadeModel(GL_SMOOTH);
	//	//cow();
	//	glCallList(m_displayListPoly);

	//}
}


void Geometry::toggleWireFrame() {
	m_wireFrameOn = !m_wireFrameOn;
}

void Geometry::createBoundingBox()
{
	_bb = BoundingBox();
	for(vec3 point : m_points)
	{
		_bb.minx = min(_bb.minx, point.x);
		_bb.miny = min(_bb.miny, point.y);
		_bb.minz = min(_bb.minz, point.z);

		_bb.maxx = max(_bb.maxx, point.x);
		_bb.maxy = max(_bb.maxy, point.y);
		_bb.maxz = max(_bb.maxz, point.z);
	}
}

BoundingBox Geometry::GetBoundingBox()
{
	return _bb;
}
