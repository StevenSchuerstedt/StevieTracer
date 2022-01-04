#include "Camera.h"


Camera::Camera() {

	// Camera
	const float aspect_ratio = 16.0 / 9.0;
	float viewport_height = 2.0;
	float viewport_width = aspect_ratio * viewport_height;
	float focal_length = 1.0;

    origin = glm::vec3(0, 0, 0);
    horizontal = glm::vec3(viewport_width, 0, 0);
    vertical = glm::vec3(0, viewport_height, 0);
    lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - glm::vec3(0, 0, focal_length);
}