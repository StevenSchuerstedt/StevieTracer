#include "lambertian.h"
#include "sampling.h"
#include "Sphere.h"
#include "Ray.h"

bool lambertian::scatter(const Ray& r_in, const isect& rec, glm::vec3& attenuation, Ray& scattered) const
{
	
	glm::vec3 scatter_direction = rec.normal + sampling::rejection_sampling_sphere();
	scattered = Ray(rec.p, scatter_direction);
	attenuation = albedo * 0.31830988618379067154f;
	return true;


}


glm::vec3 lambertian::eval() const{
	return albedo / 3.141592653f;
}

float lambertian::pdf() const {
	return 1.0f / (4 * 3.141592653f);
	//pdf for sampling cosine
	//return albedo / 3.141592653f * wi.dot(wo) * 2 * 3.141592653f;
}

float lambertian::sample(glm::vec3 w_o, glm::vec3& w_i) const
{	
	//sample cosine
	//glm::normalize(sampling::sampleCosineHemisphere());
	//return 1.0f / (4 * 3.141592653f);

	//sample sphere
	glm::normalize(sampling::sampleUniformHemisphere());
	return 1.0f / (4 * 3.141592653f);

}

int lambertian::getType() const
{
	return BSDF::lambertianBSDF;
}
