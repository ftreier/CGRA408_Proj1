#include "helper.hpp"

using namespace cgra;

mat4 Helper::LookAt(vec3 &position, vec3 &viewDir, vec3 &upDir)
{
	mat4 m = mat4::identity();

	vec3 dir = normalize(viewDir - position);
	if (length(cross(normalize(upDir), dir)) == 0)
	{
		return m;
	}
	vec3 left = normalize(cross(normalize(upDir), dir));
	vec3 newUp = cross(dir, left);

	m[3][0] = position.x;
	m[3][1] = position.y;
	m[3][2] = position.z;

	m[0][0] = left.x;
	m[0][1] = left.y;
	m[0][2] = left.z;

	m[1][0] = newUp.x;
	m[1][1] = newUp.y;
	m[1][2] = newUp.z;

	m[2][0] = dir.x;
	m[2][1] = dir.y;
	m[2][2] = dir.z;

	//auto m2 = mat4::lookAt(position, viewDir, upDir);

	return m;
}

mat4 Helper::Perspective(float fov, float hither, float yon)
{
	// create projection matrix
	mat4 m = mat4::identity();
	m[2][2] = yon / (yon - hither);
	m[3][2] = -yon*hither / (yon - hither);
	m[2][3] = 1;
	m[3][3] = 0;

	// Scale to canonical fov
	float invTanAng = 1.f / tanf(radians(fov) / 2.f);
	return mat4::scale(invTanAng, invTanAng, 1) * m;
}