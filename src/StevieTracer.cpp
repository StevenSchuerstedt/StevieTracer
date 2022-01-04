#include "StevieTracer.h"
#include <algorithm> 
#include "lambertian.h"
#include "metal.h"
#include <limits>



float clamp(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

/*
glm::vec3 StevieTracer::ray_color(Ray& r, int depth) {

	if (depth <= 0)
		return glm::vec3(0.0f, 0.0f, 0.0f);
	isect isec;
		if (scene->hit(r, 0.001f, infinity, isec)) {
			Ray scattered;
			glm::vec3 attenuation;
			
			if (isec.isLightSource)
				return glm::vec3(0.8f, 0.8f, 0.8f);
			//else if (scene->testIfInShadow(isec.p, 0.001f, infinity))
			//	return glm::vec3(0.0f, 0.0f, 0.0f);

			//sample brdf for new direction
			if (isec.ptr_mat->scatter(r, isec, attenuation, scattered)) {
				//attenuation is path throughput
				return attenuation * ray_color(scattered, depth--);
			}
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}

		
		return glm::vec3(0.1f, 0.1f, 0.1f);
	//return enviroment light
	float t = 0.5f * (glm::normalize(r.direction).y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}*/
/*
glm::vec3 StevieTracer::ray_color(Ray& r, int depth) {

	int bounces;
	glm::vec3 L(0.0f, 0.0f, 0.0f);
	glm::vec3 beta(1.0f, 1.0f, 1.0f);
	Ray ray = r;

	Sphere* lightSource = nullptr;
	for (const auto& sphere : spheres) {
		if (sphere->isLightSource)
			lightSource = sphere;
	}
	
	for (bounces = 0;; ++bounces) {
		isect isec;

		bool foundIntersection = scene->hit(ray, 0.001f, infinity, isec);

		if (bounces == 0)
			if (!foundIntersection)
				L += beta * glm::vec3(0.1f, 0.1f, 0.1f);

		if (!foundIntersection || bounces >= maxDepth) break;

		//sample illumination from lights

		glm::vec3 Li(0.0f, 0.0f, 0.0f);

		if (scene->testIfInShadow(isec.p, 0.001f, infinity))
			Li = glm::vec3(0.0f, 0.0f, 0.0f);
		else
			Li = lightSource->Le();

		L += beta * std::max(glm::dot(glm::normalize(lightSource->center - ray.origin), isec.normal), 0.0f) *  Li;
		
	
		
		//sample brdf for new direction
		Ray scattered;
		glm::vec3 attenuation;
			
		if (isec.BSDF->scatter(ray, isec, attenuation, scattered)) {
			ray = scattered;
			//brdf value and pdf are missing
			beta *= attenuation * std::max(glm::dot(ray.direction, isec.normal), 0.0f);
			//attenuation is path throughput
			//return attenuation * ray_color(scattered, depth--);
		}	

	}

	
	return L;

	return glm::vec3(0.1f, 0.1f, 0.1f);
	//return enviroment light
	float t = 0.5f * (glm::normalize(r.direction).y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}*/


glm::vec3 StevieTracer::ray_color(Ray& r, int depth) {

	int bounces;
	L = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 beta(1.0f, 1.0f, 1.0f);
	isect isec;

	Ray ray = r;

	Sphere* lightSource = nullptr;
	for (const auto& sphere : spheres) {
		if (sphere->isLightSource)
			lightSource = sphere;
	}
	for (bounces = 0;; ++bounces) {
		
		bool foundIntersection = scene->hit(ray, 0.001f, infinity, isec);

		//Russian roulette
		if (!foundIntersection || sampling::random_float() > alpha)
			break;
		beta *= 1.0f / alpha;
		if (bounces == 0 && isec.isLightSource)
			L += beta * lightSource->Le();
		
		if (isec.BSDF->getType() == BSDF::metalBSDF) {
			glm::vec3 w_o = ray.direction;
			glm::vec3 w_i(1.0f);
			isec.BSDF->normal = isec.normal;
			float BSDF_pdf = isec.BSDF->sample(w_o, w_i);
			ray = Ray(isec.p, w_i);
			bool foundIntersection = scene->hit(ray, 0.001f, infinity, isec);
			if (!foundIntersection)
				break;
			if (isec.isLightSource) 
				L += beta * lightSource->Le();
		}
			//perform NEE
			//statt rejection sampling, sample uniform hemisphere
			glm::vec3 sample(1.0f);
			float Le_pdf = lightSource->sample(sample);
			//distanz zum sample
			if (!scene->testIfInShadow(isec.p, 0.001f, glm::length(sample - isec.p) - 0.01f, sample)) {

				glm::vec3 normal = glm::normalize(sample - lightSource->center);
				glm::vec3 dir = glm::normalize(sample - isec.p);
				float geometryTerm = (std::max(glm::dot(-dir, normal), 0.0f) * std::max(glm::dot(dir, isec.normal), 0.0f)) /
					(glm::length(sample - isec.p) * glm::length(sample - isec.p));

				L += beta * lightSource->Le() *
					isec.BSDF->eval() *
					geometryTerm /
					Le_pdf;
			}
		

		
		//
		// Koordinatensysteme???
		//
		//nur obere hemissphäre samplen
		glm::vec3 w_i(1.0f);
		glm::vec3 w_o = ray.direction;
		float BSDF_pdf = isec.BSDF->sample(w_o, w_i);
		ray = Ray(isec.p, w_i);

		
		beta *= isec.BSDF->eval() *
				std::max(glm::dot(w_i, isec.normal), 0.0f) /
				BSDF_pdf;

	}

	return L;
}

void StevieTracer::write_color(std::ostream& out, glm::vec3 color, int samples_per_pixel)
{

	auto r = color.x;
	auto g = color.y;
	auto b = color.z;

	auto scale = 1.0 / samples_per_pixel;
	r = std::sqrt(scale * r);
	g = std::sqrt(scale * g);
	b = std::sqrt(scale * b);
	
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

void StevieTracer::render() {
	std::ofstream outfile("render.ppm");

	outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	Camera camera;

	int max_depth = 50;

	int samples_per_pixel = 100;

	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = 0; i < image_width; ++i) {
			glm::vec3 pixel_color(0.0f, 0.0f, 0.0f);
			
			for (int s = 0; s < samples_per_pixel; s++) {
				//create camera ray
				float u = float(i + sampling::random_float()) / (image_width - 1.0f);
				float v = float(j + sampling::random_float()) / (image_height - 1.0f);
				//L = glm::vec3(0.0f, 0.0f, 0.0f);
				Ray r = camera.getCameraRay(u, v);
				pixel_color += ray_color(r, max_depth);
			}
			
			write_color(outfile, pixel_color, samples_per_pixel);
		}
		outfile << '\n';
	}

	outfile.close();
}


void StevieTracer::createScene() {

	auto material_ground = new lambertian(glm::vec3(0.75, 0.75, 0.75));
	auto metal_sphere = new metal(glm::vec3(0.7, 0.3, 0.3));
	auto lambertian_sphere = new lambertian(glm::vec3(0.7, 0.3, 0.3));
	auto material_left = new lambertian(glm::vec3(1.0, 0.0, 0.0));
	auto material_right = new lambertian(glm::vec3(0.0, 0.0, 1.0));
	auto light_material = new lambertian(glm::vec3(1.0, 1.0, 0.0));


	Sphere* s = new Sphere(glm::vec3(0.0f, -1.0f, -3.0f), 0.5f, metal_sphere);
	spheres.push_back(s);
	Sphere* ss = new Sphere(glm::vec3(1.0f, -1.0f, -2.0f), 0.5f, lambertian_sphere);
	spheres.push_back(ss);
	Sphere* ss2 = new Sphere(glm::vec3(-1.1f, -1.0f, -2.0f), 0.5f, metal_sphere);
	spheres.push_back(ss2);

	Sphere* s0 = new Sphere(glm::vec3(0.0f, 101.5f, -1.0f), 100.0f, material_ground);
	spheres.push_back(s0);

	Sphere* s1 = new Sphere(glm::vec3(0.0f, -101.5f, -1.0f), 100.0f, material_ground);
	spheres.push_back(s1);

	Sphere* s2 = new Sphere(glm::vec3(-102.5f, 0.0f, -1.0f), 100.0f, material_left);
	spheres.push_back(s2);

	Sphere* s3 = new Sphere(glm::vec3(102.5f, 0.0f, -1.0f), 100.0f, material_right);
	spheres.push_back(s3);

	Sphere* s4 = new Sphere(glm::vec3(0.0f, 0.0f, -105.0f), 100.0f, material_ground);
	spheres.push_back(s4);

	Sphere* lightSource = new Sphere(glm::vec3(0.0f, 1.7f, -2.0f), 0.5f, light_material);
	lightSource->isLightSource = true;
	spheres.push_back(lightSource);

	

	scene = new Scene(spheres);

}

int main()
{

	StevieTracer* s = new StevieTracer();
	s->createScene();
	s->render();

	std::cout << "finished rendering hooray!" << std::endl;

}
