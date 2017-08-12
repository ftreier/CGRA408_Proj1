#pragma once
#include <string>
#include "cgra_math.hpp"

class Film
{
private:
	static const int MIN_WIDTH = 10;
	static const int MAX_WIDTH = 9999;
	static const int MIN_HEIGHT = 10;
	static const int MAX_HEIGHT = 9999;

	std::string _filename;
	int _width;
	int _height;
	unsigned char* _data;

	static unsigned char convertColor(float x);
public:

	Film(std::string filename, int width, int height);
	~Film();

	int GetHeight();
	int GetWidth();
	void writeTest();
	void SetColor(int x, int y, cgra::vec3 color);
	void SetColor(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	void WriteImage();
};
