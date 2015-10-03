#pragma once
#ifndef MESH_H_
#define MESH_H_

#include "resource/context/BaseContextResource.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace Resource {
	namespace Context {
		struct Vertex {
			glm::vec3 pos;
			glm::vec3 nor;
			glm::vec2 uv;
			glm::vec3 tan;
			glm::vec3 bittan;
		};

		class Mesh
		{
		public:
			Mesh();
			~Mesh();


		private:
			/* Mesh Data */
			std::vector<Vertex> verts;
			std::vector<unsigned int> indicies;
			Resource::Context::Material material;
		};
	}
}

#endif