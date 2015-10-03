#pragma once
#ifndef MESH_H_
#define MESH_H_

#include "resource/context/BaseContextResource.hpp"
#include "resource/context/Material.hpp"

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

		class ContextMesh
		{
		public:
			ContextMesh();
			~ContextMesh();

			// Build mesh buffers from data
			void BuildMesh(std::vector<Vertex> verts, std::vector<GLuint> indicies, std::shared_ptr<Resource::ResourceHandle<Resource::Context::Material>> material);
			// Bind this mesh's VAO
			void bind();

			// STATIC - Unbind all meshes (Unbinds active vertex array)
			static void unbindAll() 
			{
				glBindVertexArray(0);
			}

			// Binds mesh VAO and draws. NOTE: Does not unbind mesh!
			void draw();

			Resource::ResourceHandle<Resource::Context::Material>* getMaterial();
		private:
			/* Mesh Data */
			unsigned int _indexCount;
			std::shared_ptr<Resource::ResourceHandle<Resource::Context::Material>> _material;
			GLuint VAO, VBO, EBO;
		};
	}
}

#endif