#pragma once
#include "shape.hpp"
#include "distantLight.hpp"

class Renderer
{
public:
	static const int MAX_RAYCAST_DEPTH = 5;


	Renderer();
	~Renderer();
	static void readLine(int& size, int& c, char*& buffer, FILE* file);
	static std::vector<std::string> Split(std::string& cs);

	static std::string basicTrim(std::string part);
	static void processCommand(std::string& cs);
	static void ParesFile(std::string& cs);
	static void Render(std::string);

	static std::vector<Light*> GetLights();
	static std::vector<Shape*> GetShapes();
private:
	static void cleanup();
};
