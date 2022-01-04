#pragma once
#include "glm/glm.hpp"
#include "Ray.h"


class Material;

struct isect {
	glm::vec3 p;
	glm::vec3 normal;
	float t;
	Material* BSDF;
	bool isLightSource = false;
};

class Sphere {

	float brightness = 2.0f;

public:
	Sphere(glm::vec3 c, float r, Material* m)
		: center(c), radius(r), BSDF(m)
	{}

	bool hit(Ray& r, float t_min, float t_max, isect& i);

	glm::vec3 Le();

	float pdf();

	float sample(glm::vec3& w);

	glm::vec3 center;
	float radius;
	Material* BSDF;

	bool isLightSource = false;
};