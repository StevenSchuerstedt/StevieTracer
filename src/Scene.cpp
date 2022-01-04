#include "Scene.h"
#include "Ray.h"
#include "Sphere.h"


bool Scene::hit(Ray& r, float t_min, float t_max, isect& i)
{
	isect temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& sphere : spheres) {
		if (sphere->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			i = temp_rec;
		}
	}

	return hit_anything;
}

bool Scene::testIfInShadow(glm::vec3 position, float t_min, float t_max, glm::vec3 sample)
{
	Sphere* lightSource = nullptr;
	for (const auto& sphere : spheres) {
		if (sphere->isLightSource)
			lightSource = sphere;
	}
	isect isec;
	glm::vec3 direction = glm::normalize(sample - position);
	Ray r(position, direction);
	for (const auto& sphere : spheres) {
		if (sphere->hit(r, t_min, t_max, isec)) {
			if (!isec.isLightSource)
				return true;
		}
	}
	return false;

}
