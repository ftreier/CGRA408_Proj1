#pragma once

class Shape;

#include "shape.hpp"

class Material
{
public:
	Material(cgra::vec3 diffColor, cgra::vec3 specColor, float, float, float);
	cgra::vec4 reflect(const cgra::vec4& d, const cgra::vec4& n);
	cgra::vec4 refract(cgra::vec4 direction, cgra::vec4 normal, float n1, float n2);
	cgra::vec3 CalculateColor(Ray* r, Shape* s);

private:
	cgra::vec3 _difuseColor;
	cgra::vec3 _specColor;
	float _reflectionFactor;
	float _refractionFactor;
	float _refractionIndex;

	std::uniform_real_distribution<float> _randomDistribution;
	std::mt19937 _randomGenerator;

	float rn();
};

Material* CreateRedPlastic();
Material* CreateGlass();