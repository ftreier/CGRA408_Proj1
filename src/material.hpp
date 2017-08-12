#pragma once

class Shape;

#include "shape.hpp";

class Material
{
public:
	Material(cgra::vec3 color, float, float, float);
	cgra::vec4 reflect(const cgra::vec4& d, const cgra::vec4& n);
	cgra::vec4 refract(cgra::vec4 direction, cgra::vec4 normal, float n2);
	cgra::vec3 CalculateColor(Ray* r, Shape* s);

private:
	cgra::vec3 _difuseColor;
	float _reflectionFactor;
	float _refractionFactor;
	float _refractionIndex;
};

Material* CreateRedPlastic();
Material* CreateGlass();