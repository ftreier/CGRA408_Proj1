#pragma once
#include "cgra_math.hpp"

class Helper
{
public:
	static cgra::mat4 LookAt(cgra::vec3 &position, cgra::vec3 &viewDir, cgra::vec3 &updir);
	static cgra::mat4 Perspective(float fov, float hiter, float yon);
};
