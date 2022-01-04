#pragma once
#include "Material.h"
#include "glm/glm.hpp"

class lambertian : public Material {
public:
	lambertian(const glm::vec3 a) : albedo(a) {}

	virtual bool scatter(const Ray& r_in, const isect& rec, glm::vec3& attenuation, Ray& scattered) const override;

	virtual glm::vec3 eval() const override;
	virtual float pdf() const override;
	virtual float sample(glm::vec3 w_o, glm::vec3& w_i) const override;
	virtual int getType() const override;
	glm::vec3 albedo;
};