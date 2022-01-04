#pragma once
#include <vector>
#include "glm/glm.hpp"
class Ray;
class isect;
class Sphere;

class Scene {
public:
	Scene(){}
	Scene(std::vector<Sphere*> spheres) : spheres(spheres) {}

	bool hit(Ray& r, float t_min, float t_max, isect& i);

	bool testIfInShadow(glm::vec3 position, float t_min, float t_max, glm::vec3 sample);

	std::vector<Sphere*> spheres;
};