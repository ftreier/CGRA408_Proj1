#include "film.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "cgra_math.hpp"

using namespace std;

Film::Film(string filename, int width, int height)
{
	if (!filename.empty())
	{
		_filename = filename;
	}
	else
	{
		_filename = "scene.jpg";
	}

	// check width boundary
	if(width < MIN_WIDTH)
	{
		_width = MIN_WIDTH;
	} else if (width > MAX_WIDTH)
	{
		_width = MAX_WIDTH;
	}
	else
	{
		_width = width;
	}

	// check height boundary
	if (height < MIN_HEIGHT)
	{
		_height = MIN_HEIGHT;
	}
	else if (height > MAX_HEIGHT)
	{
		_height = MAX_HEIGHT;
	}
	else
	{
		_height = height;
	}

	_data = new unsigned char[_height * _width * 3];// {0};
}

Film::~Film()
{
	//delete[] _data;
}

int Film::GetHeight()
{
	return _height;
}

int Film::GetWidth()
{
	return _width;
}


void Film::writeTest()
{
	char data[12];
	for (int i = 0; i < 12; i++)
	{
		//if(i % 3 == 1)
		//{
		//	data[i] = 255;
		//}
		//else
		//{
		//	data[i] = 0;
		//}
		data[i] = 255 / (i + 1);
	}

	_filename = "asdf.jpg";

	stbi_write_jpg(_filename.c_str(), 2, 2, 3, data, 100);
	stbi_write_bmp("Test.bmp", 2, 2, 3, data);
}

unsigned char Film::convertColor(float x)
{
	if(x >= 1)
	{
		return 255;
	}
	
	if(x <= 0)
	{
		return 0;
	}

	return static_cast<unsigned char>(x * 255);
}

void Film::SetColor(int x, int y, cgra::vec3 color)
{
	SetColor(x, y, convertColor(color.r), convertColor(color.g), convertColor(color.b));
}

void Film::SetColor(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	int index = (x * GetWidth() + y) * 3;
	_data[index] = r;
	_data[index + 1] = g;
	_data[index + 2] = b;
}

void Film::WriteImage()
{
	stbi_write_jpg(_filename.c_str(), GetWidth(), GetHeight(), 3, _data, 100);
	//stbi_write_bmp(_filename.c_str(), GetWidth(), GetHeight(), 3, _data);
}
