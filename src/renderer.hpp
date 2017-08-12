#pragma once
#include "shape.hpp"
#include "distantLight.hpp"

class Renderer
{
public:
	static const int MAX_RAYCAST_DEPTH = 5;

	Renderer();
	~Renderer();

	void Render();

	static std::vector<Light*> GetLights();
	static std::vector<Shape*> GetShapes();
private:
};
