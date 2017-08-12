#pragma once
#include "cgra_math.hpp"
#include "film.hpp"
#include "ray.hpp"

struct Sample {
	float imageX, imageY;
//	float lensU, lensV;
//	float time;
};

class Camera
{
public:

	Camera(cgra::mat4& viewToWorld, cgra::mat4& camToScreen, Film film);
	~Camera();

	Ray GenerateRay(Sample s);

private:
	cgra::mat4 _camToWorld;
	cgra::mat4 _camToScreen;
	cgra::mat4 _screenToRaster;
	cgra::mat4 _rasterToScreen;
	cgra::mat4 _rasterToCamera;

	cgra::vec4 rasterToCamera(cgra::vec4 raster);
	//Ray cameraToWorld(Ray);
	//cgra::vec4 MatrixVector(cgra::mat4 m, cgra::vec4 v);
};

Camera* CreateCamera(cgra::vec3 &position, cgra::vec3 &viewingDir, cgra::vec3 &upDir, float fov, Film f);