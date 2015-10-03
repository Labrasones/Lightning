#include "resource/context/ContextMesh.hpp"

using namespace Resource;
using namespace Context;


ContextMesh::ContextMesh()
{}

ContextMesh::~ContextMesh()
{}

void ContextMesh::BuildMesh(std::vector<Vertex> verts, std::vector<GLuint> indicies, std::shared_ptr<Resource::ResourceHandle<Resource::Context::Material>> material)
{
	// Store the material used by the mesh
	_material = material;
	// Store the number of indicies
	_indexCount = indicies.size();

	// Generate Buffers/Arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Setup the render state
	glBindVertexArray(VAO); // Bind the VAO to store it in

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the VBO to load data into
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW); // Load the data into the VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind the EBO to load data into
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), &indicies[0], GL_STATIC_DRAW);

	// Set up vertex attributes
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, nor));
	// Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
	// Tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tan));
	// Bit Tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bittan));

	// Unbind the arrays and buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ContextMesh::bind()
{
	glBindVertexArray(VAO);
}

void ContextMesh::draw()
{
	bind();
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}

Resource::ResourceHandle<Resource::Context::Material>* ContextMesh::getMaterial()
{
	return _material.get();
}