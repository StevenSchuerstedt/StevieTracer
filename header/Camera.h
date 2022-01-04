#pragma once
#include "glm/glm.hpp"
#include "Ray.h"
class Camera {
public:
	Camera();


	Ray getCameraRay(float u, float v) {
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	glm::vec3 origin;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 lower_left_corner;

};