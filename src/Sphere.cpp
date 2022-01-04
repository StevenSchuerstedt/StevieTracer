#include "Sphere.h"
#include "sampling.h"

bool Sphere::hit(Ray& r, float t_min, float t_max, isect& i)
{
	glm::vec3 oc = r.origin - center;
	auto a = glm::length(r.direction) * glm::length(r.direction);
	auto half_b = glm::dot(oc, r.direction);
	auto c = (glm::length(oc) * glm::length(oc)) - (radius * radius);
	auto discriminant = half_b * half_b - a * c;

	if (discriminant > 0) {
		auto root = sqrt(discriminant);

		auto temp = (-half_b - root) / a;
		if (temp < t_max && temp > t_min) {
			i.t = temp;
			i.p = r.at(i.t);
			i.normal = (i.p - center) / radius;
			i.BSDF = BSDF;
			i.isLightSource = isLightSource;
			return true;
		}

		temp = (-half_b + root) / a;
		if (temp < t_max && temp > t_min) {
			i.t = temp;
			i.p = r.at(i.t);
			i.normal = (i.p - center) / radius;
			i.BSDF = BSDF;
			i.isLightSource = isLightSource;
			return true;
		}
	}

	return false;

}

glm::vec3 Sphere::Le()
{
	return glm::vec3(brightness, brightness, brightness);
}

float Sphere::sample(glm::vec3& w) {

	w = glm::normalize(sampling::rejection_sampling_sphere())* radius + center;

	//return pdf
	return 1.0f / (4 * 3.141592653f * radius * radius);

}

float Sphere::pdf() {
	return 1.0f / (4 * 3.141592653f * radius * radius);
}