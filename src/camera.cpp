#include "camera.hpp";
#include "helper.hpp";
#include "film.hpp"

using namespace cgra;

Camera::Camera(mat4 &viewToWorld, mat4 &camToScreen, Film film)
{
	_camToWorld = viewToWorld;
	_camToScreen = camToScreen;

	// ScreenToRaster = Scale(xResolution, yResolution, 1.f) * Scale(1.f / (screenWindow[1] - screenWindow[0]), 1.f / (screenWindow[2] - screenWindow[3]), 1.f) * Translate(Vector(-screenWindow[0], -screenWindow[3], 0.f));
	mat4 t = mat4::translate(1.0f, -1.0f, 0.0f);
	mat4 t2 = mat4::scale(1.0f / 2.0f, 1.0f / -2.0f, 1.0f) * t;
	mat4 t3 = mat4::scale(film.GetWidth(), film.GetHeight(), 1.0f) * t2;
	_screenToRaster = mat4::scale(float(film.GetWidth()), float(film.GetHeight()), 1.f) * mat4::scale(1.f / 2.f, 1.0 / -2.f, 1.f) * mat4::translate(1.f, -1.f, 0.f);
	_rasterToScreen = inverse(_screenToRaster);
	mat4 matrix4 = inverse(_camToScreen);
	_rasterToCamera = matrix4 * _rasterToScreen;
}

Camera::~Camera()
{
}

Ray Camera::GenerateRay(Sample s)
{
	vec4 direction = rasterToCamera(vec4(s.imageX, s.imageY, 0, 1));
	direction.w = 0;
	direction = normalize(direction);
	vec4 origin = vec4(0, 0, 0, 1);
	Ray ray = Ray(origin, direction, 0);
	ray.ConvertToSpace(_camToWorld);
	return ray;
}

vec4 Camera::rasterToCamera(vec4 raster)
{
	return normalize(_rasterToCamera * raster);
}

//Ray Camera::cameraToWorld(Ray r)
//{
//	vec4 worldOrig = (_camToWorld * r.GetOrigin());
//	//vec4 worldOrig = normalize(_camToWorld * r.GetOrigin());
//	vec4 worldDirection = normalize(_camToWorld * r.GetDirection());
//	return Ray(worldOrig, worldDirection, r.GetCount());
//}

//vec4 Camera::MatrixVector(mat4 m, vec4 v)
//{
//	float x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];
//	float y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];
//	float z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];
//	float w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3];
//	return vec4(x, y, z, w);
//}
Camera* CreateCamera(vec3 &position, vec3 &viewingDir, vec3 &upDir, float fov, Film film)
{
	const float HITHER = 1e-2f;
	const float YON = 1000.0f;
	
	mat4 camToWorld = Helper::LookAt(position, viewingDir, upDir);
	mat4 camToScreen = Helper::Perspective(fov, HITHER, YON);
	return new Camera(camToWorld, camToScreen, film);
}