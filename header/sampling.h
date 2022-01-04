#pragma once
#include "glm/glm.hpp"
#include <random>


class sampling {

public:
	static float random_float() {
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);
	}

	static float random_float( float min , float max) {
		static std::uniform_real_distribution<double> distribution(min, max);
		static std::mt19937 generator;
		return distribution(generator);
	}

	static glm::vec3 random() {
		return glm::vec3(random_float(), random_float(), random_float());
	}

	static glm::vec3 random(float min, float max) {
		return glm::vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}

	static glm::vec3 rejection_sampling_sphere() {
		while (true) {
			auto p = random(-1, 1);
			if (glm::length(p) * glm::length(p) >= 1) continue;
			return p;
		}
	}

	static glm::vec3 random_unit_vector() {
		auto a = random_float(0, 2 * std::_Pi);
		auto z = random_float(-1, 1);
		auto r = std::sqrt(1 - z * z);
		return glm::vec3(r * cos(a), r * sin(a), z);
	}


	static glm::vec3 sampleUniformHemisphere() {
		float z = random_float();
		float tmp = sqrtf(1.0f - z * z);

		float sinPhi = sinf(2.0f * 3.141592653f * random_float());
		float cosPhi = cosf(2.0f * 3.141592653f * random_float());

		return glm::vec3(cosPhi * tmp, sinPhi * tmp, z);

	}

	static glm::vec2 sampleUniformDisk()
	{
		float radius = std::sqrt(random_float());
		float sinPhi = std::sin(2 * 3.141592653f * random_float());
		float cosPhi = std::cos(2 * 3.141592653f * random_float());

		return glm::vec2(
			cosPhi * radius,
			sinPhi * radius
		);
	}

	static glm::vec3 sampleCosineHemisphere()
	{
		glm::vec2 p = sampleUniformDisk();
		float sq = 1.0f - p.x * p.x - p.y * p.y;
		float z = 1e-10 < sq ? std::sqrt(1.0f - p.x * p.x - p.y * p.y) : 1e-10;
		return glm::vec3(p.x, p.y, z);
	}

};