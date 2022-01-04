#include "metal.h"
#include "Sphere.h"
#include "Sphere.h"
bool metal::scatter(const Ray& r_in, const isect& rec, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction), rec.normal);

	scattered = Ray(rec.p, reflected);
	attenuation = albedo;

	return (glm::dot(scattered.direction, rec.normal) > 0);
}

glm::vec3 metal::eval() const
{
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

float metal::pdf() const
{
	return 1.0f;
}

float metal::sample(glm::vec3 w_o, glm::vec3& w_i) const
{
	 
	w_i = glm::reflect(w_o, normal);
	return 1.0f;
}

int metal::getType() const
{
	return BSDF::metalBSDF;
}
