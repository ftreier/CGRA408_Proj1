#include "renderer.hpp"
#include "cgra_math.hpp"
#include "film.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "shape.hpp"

using namespace cgra;
using namespace std;

static vector<Shape*> _shapes;
static vector<Light*> _lights;

Renderer::Renderer()
{
	//_shapes = vector<Shape>();
}

Renderer::~Renderer()
{
	
}

void Renderer::Render()
{
	Film f = Film("Asdf.jpg", 400, 400);
	auto camera = CreateCamera(vec3(0.f, 10.f, 100.f), vec3(0, -1, 0), vec3(0, 1, 0), 30, f);
	//Shape sp = Sphere(1, mat4::identity(), mat4::identity());
	//_shapes.push_back(CreateSphere(20, vec3(0, 7, -20), CreateRedPlastic()));
	_shapes.push_back(CreateSphere(20, vec3(0, 7, -20), CreateGlass()));
	_shapes.push_back(CreateSphere(20, vec3(0, 0, -40), CreateRedPlastic()));
	//_shapes.push_back(CreateSphere(10, vec3(0, -25, -22), CreateRedPlastic()));
	_lights.push_back(CreateDistantLight(vec4(0, 1, 0, 0), vec3(1)));
	Sample s;
	for (int i = 0; i < f.GetWidth(); i++)
	{
		for (int j = 0; j < f.GetHeight(); j++)
		{
			s.imageX = i;
			s.imageY = j;
			Ray r = camera->GenerateRay(s);

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

			if(closestShape != nullptr)
			{
				closestShape->Intersect(&r);
				f.SetColor(i, j, closestShape->CalculateLight(&r));
			}
		}
	}

	f.WriteImage();
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
