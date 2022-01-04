#pragma once
#include "glm/glm.hpp"
#include "Ray.h"
#include "Sphere.h"
#include "Camera.h"
#include "sampling.h"
#include "Material.h"
#include "Scene.h"
#include <iostream>
#include <fstream>  
#include <vector>

class StevieTracer {

	const float infinity = std::numeric_limits<double>::infinity();
	// Image
	const float aspect_ratio = 16.0 / 9.0;
	const int image_width = 500;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	int maxDepth = 50;

	void write_color(std::ostream& out, glm::vec3 color, int samples_per_pixel);
	glm::vec3 ray_color(Ray& r, int depth);

	std::vector<Sphere*> spheres;

	Scene* scene = nullptr;
	float alpha = 0.8f;

	glm::vec3 L = glm::vec3(0.0f, 0.0f, 0.0f);

public:
	void render();
	void createScene();
};

