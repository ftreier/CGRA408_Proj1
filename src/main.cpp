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
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "cgra_math.hpp"
#include "geometry.hpp"
#include "film.hpp"
#include "camera.hpp"
#include "renderer.hpp"

using namespace std;
using namespace cgra;

// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.1f;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_leftMouseDown = false;
vec2 g_mousePosition;
float g_pitch = 0;
float g_yaw = 0;
float g_zoom = 1.0;
const float MIN_ZOOM = 0.021f;


// Geometry loader and drawer
//
Geometry *g_geometry = nullptr;


float checkBounds(float value, float minValue, float maxValue)
{
	if (maxValue == minValue)
	{
		throw invalid_argument("MinValue and MaxValue must not be the same.");
	}
	if (minValue > maxValue)
	{
		throw invalid_argument("MinValue must be smasser then MaxValue.");
	}

	if(value > maxValue)
	{
		value = value - maxValue + minValue;
	}
	else if (value < minValue)
	{
		value = value + maxValue - minValue;
	}

	return value;
}

float chekcDegreeBounds(float value)
{
	return checkBounds(value, 0, 360);
}

// Sets up where and what the light is
// 
void setupLight()
{
	// No transform for the light
	// makes it move realitive to camera
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//vec4 direction(0.0, 0.0, 1.0, 0.0);
	//vec4 diffuse(0.7f, 0.7f, 0.7f, 1.0);
	//vec4 ambient(0.2f, 0.2f, 0.2f, 1.0);

	//glLightfv(GL_LIGHT0, GL_POSITION, direction.dataPointer());
	//glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse.dataPointer());
	//glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient.dataPointer());
	//
	//glEnable(GL_LIGHT0);
}


// Sets up where the camera is in the scene
// 
void setupCamera(int width, int height)
{
	//// Set up the projection matrix
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(g_fovy, width / float(height), g_znear, g_zfar);

	//// Set up the view part of the model view matrix
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(	0.0, 0.0, 50.0,// position of camera
	//			0.0, 0.0, 0.0, // position to look at
	//			0.0, 1.0, 0.0);// up relative to camera

	//// yaw, pitch and zoom
	//glRotatef(g_yaw, 0, 1, 0);
	//glRotatef(g_pitch, 1, 0, 0);
	//glScalef(g_zoom, g_zoom, g_zoom);
}


// Render one frame to the current window given width and height
//
void render(int width, int height)
{
	//// Set viewport to be the whole window
	//glViewport(0, 0, width, height);

	//// Setup light
	//setupLight();

	//// Setup camera
	//setupCamera(width, height);

	//// Grey/Blueish background
	//glClearColor(0.3f,0.3f,0.4f,1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Enable flags for normal rendering
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);

	//// Set the current material (for all objects) to red
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
	//glColor3f(1.0f, 0.0f, 0.0f); //red

	//// Render geometry
	//g_geometry->renderGeometry();

	//// Disable flags for cleanup (optional)
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
	//glDisable(GL_NORMALIZE);
	//glDisable(GL_COLOR_MATERIAL);
}

//Main program
// 
int main(int argc, char **argv)
{
	Renderer().Render();
	//Film f = Film("Asdf.jpg", 200, 200);
	//CreateCamera(vec3(0, 10, 100), vec3(0, -1, 0), vec3(0, 1, 0), 30, f);
//	f.writeTest();
	// Check argument list
	//if(argc != 2){
	//	cout << "Obj filename expected, eg:" << endl << "    ./a1 teapot.obj" << endl;
	//	abort(); // Unrecoverable error
	//}

	// Initialize out scene
	//g_geometry = new Geometry(argv[1]);
}