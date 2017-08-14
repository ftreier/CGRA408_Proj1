#include "renderer.hpp"
#include "cgra_math.hpp"
#include "film.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "shape.hpp"
#include "plain.hpp"
#include <iterator>

using namespace cgra;
using namespace std;

static vector<Shape*> _shapes;
static vector<Light*> _lights;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::readLine(int& size, int& c, char*& buffer, FILE* file)
{
	int pos = 0;
	do
	{ // read one line
		c = fgetc(file);
		if (c != EOF) buffer[pos++] = static_cast<char>(c);
		if (pos >= size - 1)
		{ // increase buffer length - leave room for 0
			size *= 2;
			buffer = static_cast<char*>(realloc(buffer, size));
		}
	} while (c != EOF && c != '\n');
	buffer[pos] = 0;
}

vector<string> Renderer::split(string& cs)
{
	vector<string> tokens;
	istringstream iss(cs);
	copy(istream_iterator<string>(iss),
	     istream_iterator<string>(),
	     back_inserter(tokens));
	return tokens;
}

vec3 _lookAt;
vec3 _upDir;
vec3 _viwDir;
vec3 _rotation;
vec3 _translation;
int _fov = 30;
Camera* _camera;
Film* _film;
Material* _material;
int _pixelsamples;

string Renderer::basicTrim(string part)
{
	while (part.length() > 0 && (part[0] == '[' || part[0] == ' ' || part[0] == '"'))
	{
		part.erase(0, 1);
	}
	while (part.length() > 0 && (part[part.length()-1] == ']' || part[part.length() - 1] == '"' || part[part.length() - 1] == ']'))
	{
		part.erase(part.size() - 1, 1);
	}

	return part;
}

void Renderer::processCommand(string& cs)
{
	vector<string> tokens = split(cs);

	if(tokens.size() > 0)
	{
		auto token = tokens.at(0);
		if (token.find("LookAt") == 0)
		{
			_lookAt = vec3(stof(tokens.at(1)), stof(tokens.at(2)), stof(tokens.at(3)));
			_viwDir = vec3(stof(tokens.at(4)), stof(tokens.at(5)), stof(tokens.at(6)));
			_upDir = vec3(stof(tokens.at(7)), stof(tokens.at(8)), stof(tokens.at(9)));
		}
		else if (token.find("Camera") == 0)
		{
			for (int i = 1; i < tokens.size()- 1; i++)
			{
				if(basicTrim(tokens.at(i)).find("fov") == 0)
				{
					auto part = tokens.at(i+1);

					_fov = stoi(basicTrim(part));
					break;
				}
			}
		}
		else if (token.find("Sampler") == 0)
		{
			int type = 0;
			int pixelSamples = 1;

			for (int i = 1; i < tokens.size(); i++)
			{
				auto part = basicTrim(tokens.at(i));

				if (part.find("random"))
				{
					type = 1;
				}
				else if (part.find("pixelsamples") == 0)
				{
					pixelSamples = stoi(basicTrim(tokens.at(++i)));
				}
			}

			switch (type)
			{
			case 1:
				_pixelsamples = pixelSamples;
				break;
			default:
				cout << "Not supported sampler detected" << endl;
			}
		}
		else if (token.find("Film") == 0)
		{
			string filename = "test.jpg";
			int xres = 400;
			int yres = 400;
			for (int i = 1; i < tokens.size() - 1; i++)
			{
				auto part = basicTrim(tokens.at(i));
				if (part.find("filename") == 0)
				{
					filename = basicTrim(tokens.at(++i));
				}
				else if (part.find("xresolution") == 0)
				{
					xres = stoi(basicTrim(tokens.at(++i)));
				}
				else if (part.find("yresolution") == 0)
				{
					yres = stoi(basicTrim(tokens.at(++i)));
				}
			}
			_film = new Film(filename, xres, yres);
		}
		else if (token.find("LightSource") == 0)
		{
			vec3 from;
			vec4 to;
			vec3 color;
			int type = 0;

			for (int i = 1; i < tokens.size(); i++)
			{
				auto part = basicTrim(tokens.at(i));
				if (part.find("distant") == 0)
				{
					type = 1;
				}
				else if (part.find("from") == 0)
				{
					from = vec3(stof(basicTrim(tokens.at(i + 1))), stof(basicTrim(tokens.at(i + 2))), stof(basicTrim(tokens.at(i + 3))));
					i += 3;
				}
				else if (part.find("to") == 0)
				{
					to = vec4(stof(basicTrim(tokens.at(i + 1))), stof(basicTrim(tokens.at(i + 2))), stof(basicTrim(tokens.at(i + 3))), 0);
					i += 3;
				}
				else if (part.find("L") == 0)
				{
					color = vec3(stof(basicTrim(tokens.at(i + 1))), stof(basicTrim(tokens.at(i + 2))), stof(basicTrim(tokens.at(i + 3))));
					i += 3;
				}
			}

			switch (type)
			{
			case 1:
				_lights.push_back(CreateDistantLight(to, color));
				break;
			default: 
				cout << "Not supported light source detected" << endl;
			}
		}
		else if (token.find("Rotate") == 0)
		{
			float angle = stof(basicTrim(tokens.at(1))) * math::pi() / 180.0f;

			if(tokens.at(2)[0] == '1')
			{
				_rotation.x += angle;
			}
			if (tokens.at(3)[0] == '1')
			{
				_rotation.y += angle;
			}
			if (tokens.at(4)[0] == '1')
			{
				_rotation.z += angle;
			}
		}
		else if (token.find("Translate") == 0)
		{
			_translation.x = stof(basicTrim(tokens.at(1)));
			_translation.y = stof(basicTrim(tokens.at(2)));
			_translation.z = stof(basicTrim(tokens.at(3)));
		}
		else if (token.find("Material") == 0)
		{
			vec3 color;
			vec3 diffColor;
			float reflection = 0.2;
			float refraction = 0;
			float refractionIndex = 1;

			for (int i = 1; i < tokens.size() - 1; i++)
			{
				auto part = basicTrim(tokens.at(i));
				if (part.find("color") == 0)
				{
					color = vec3(stof(basicTrim(tokens.at(i + 1))), stof(basicTrim(tokens.at(i + 2))), stof(basicTrim(tokens.at(i + 3))));
					i += 3;
				}
				if (part.find("diffColor") == 0)
				{
					diffColor = vec3(stof(basicTrim(tokens.at(i + 1))), stof(basicTrim(tokens.at(i + 2))), stof(basicTrim(tokens.at(i + 3))));
					i += 3;
				}
				else if (part.find("Kr") == 0)
				{
					reflection = stof(basicTrim(tokens.at(++i)));
				}
				else if (part.find("Kt") == 0)
				{
					refraction = stof(basicTrim(tokens.at(++i)));
				}
				else if (part.find("eta") == 0)
				{
					refractionIndex = stof(basicTrim(tokens.at(++i)));
				}
			}

			_material = new Material(color, diffColor, reflection, refraction, refractionIndex);
		}
		else if (token.find("Shape") == 0)
		{
			int type = 0;
			float param1 = 1;
			float param2 = 1;

			for (int i = 1; i < tokens.size(); i++)
			{
				auto part = basicTrim(tokens.at(i));
				if (part.find("sphere") == 0)
				{
					type = 1;
				}
				if (part.find("plain") == 0)
				{
					type = 2;
				}
				else if (part.find("radius") == 0)
				{
					param1 = stof(basicTrim(tokens.at(++i)));
				}
				else if (part.find("width") == 0)
				{
					param1 = stof(basicTrim(tokens.at(++i)));
				}
				else if (part.find("height") == 0)
				{
					param2 = stof(basicTrim(tokens.at(++i)));
				}
			}

			switch (type)
			{
			case 1:
				_shapes.push_back(CreateSphere(param1, _translation, _material));
				break;
			case 2:
				_shapes.push_back(CreatePlain(param1, param2, _translation, _rotation, _material));
				break;
			default:
				cout << "Not supported shape detected" << endl;
			}
		}
	}
}

void Renderer::paresFile(string& cs)
{
	int size = 1024;
	int c;
	char *buffer = static_cast<char *>(malloc(size));

	auto file = fopen(cs.c_str(), "r");
	if(file != nullptr)
	{
		try
		{
			string command = "";
			string nextCommand = "";
			int noOfSpaces = 0;

			do
			{
				readLine(size, c, buffer, file);
				if (buffer[0] == '#')
				{
					continue;
				}

				for (int i = 0; i < size; i++)
				{
					if (buffer[i] != ' ')
					{
						if (i > noOfSpaces)
						{
							command += string(buffer);
						}
						else
						{
							nextCommand = string(buffer);
						}

						i = size;
					}
				}

				if (nextCommand != "")
				{
					try
					{
						processCommand(command);

					}
					catch (const exception& ex)
					{
						cout << "Error processing command: " << command << endl;
						cout << "Error message: " << ex.what();
					}	
					command = "";
					if (nextCommand.find("AttributeBegin") != string::npos)
					{
						noOfSpaces += 2;
					}
					else if (nextCommand.find("AttributeEnd") != string::npos)
					{
						noOfSpaces -= 2;
						_translation = vec3(0);
						_rotation = vec3(0);
					}
					else
					{
						command = nextCommand;
					}

					nextCommand = "";
				}
			} while (c != EOF);

		}
		catch (exception e)
		{
			cout << "Error parsing file." << endl;
			cout << "Error message: " << e.what();
			fclose(file);
		}

		_camera = CreateCamera(_lookAt, _viwDir, _upDir, _fov, _film);
	}
}

void Renderer::cleanup()
{
	for (Light* light : _lights)
	{
		delete light;
	}
	_lights.clear();
	for (Shape* shape : _shapes)
	{
		delete shape;
	}
	_shapes.clear();

	if (_film != nullptr)
	{
		delete _film;
		_film = nullptr;
	}

	if (_camera != nullptr)
	{
		delete _camera;
		_camera = nullptr;
	}

	if(_material != nullptr)
	{
		delete _material;
		_material = nullptr;
	}
}

void Renderer::Render(string configFile)
{
	paresFile(configFile);
	//int samples = 1;
	//Film* f = new Film("Asdf.jpg", 400, 400);
	//auto camera = CreateCamera(vec3(0.f, 10.f, 100.f), vec3(0, -1, 0), vec3(0, 1, 0), 30, f);
	////Shape sp = Sphere(1, mat4::identity(), mat4::identity());
	////_shapes.push_back(CreateSphere(20, vec3(5, 7, -20), CreateRedPlastic()));
	//_shapes.push_back(CreateSphere(20, vec3(10, 7, -20), CreateGlass()));
	////_shapes.push_back(CreateSphere(20, vec3(0, 0, -40), CreateRedPlastic()));
	//_shapes.push_back(CreatePlain(40, 40, vec3(-12, 0, -10), vec3(0,1,0), CreateRedPlastic()));
	////_shapes.push_back(CreateSphere(10, vec3(0, -25, -22), CreateRedPlastic()));
	//_lights.push_back(CreateDistantLight(vec4(0, 1, 1, 0), vec3(1)));
	Sample s;
	for (int i = 0; i < _film->GetWidth(); i++)
	{
		for (int j = 0; j < _film->GetHeight(); j++)
		{
			vec3 color = vec3(0);
			for (int k = 0; k < _pixelsamples; k++)
			{
				if(k== 0)
				{
					s.imageX = i;
					s.imageY = j;
				}
				else
				{
					s.imageX = i + math::random(-0.5f, 0.5f);
					s.imageY = j + math::random(-0.5f, 0.5f);;
				}
				
				Ray r = _camera->GenerateRay(s);

				float minDist = numeric_limits<float>::max();
				Shape* closestShape = nullptr;

				for (Shape* shape : _shapes)
				{
					// ToDo: pick closest
					if (shape->Intersect(&r) && r.GetDistance() < minDist)
					{
						closestShape = shape;
						minDist = r.GetDistance();
					}
				}

				if (closestShape != nullptr)
				{
					closestShape->Intersect(&r);
					color += closestShape->CalculateLight(&r);
				}

			}

			_film->SetColor(i, j, color / _pixelsamples);
		}
	}

	_film->WriteImage();

	cleanup();
}

vector<Light*> Renderer::GetLights()
{
	return _lights;
}

vector<Shape*> Renderer::GetShapes()
{
	return _shapes;
}

//std::vector<Shape*> Renderer::GetShapes()
//{
//	return _shapes;
//}
