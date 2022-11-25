#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	void Draw(Shader shader);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

