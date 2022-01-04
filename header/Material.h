#pragma once
#include "Ray.h"
#include "glm/glm.hpp"
#include "Sphere.h"

enum BSDF { metalBSDF, lambertianBSDF };

class Material {
public:
	virtual bool scatter(
		const Ray& r_in, const isect& rec, glm::vec3& attenuation, Ray& scattered
	) const = 0;

	virtual glm::vec3 eval() const = 0;
	virtual float pdf() const = 0;
	virtual float sample(glm::vec3 w_o, glm::vec3& w_i) const = 0;
	virtual int getType() const = 0;
	glm::vec3 normal = glm::vec3(1.0f, 1.0f, 1.0f);
};