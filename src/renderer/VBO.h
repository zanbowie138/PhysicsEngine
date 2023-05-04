#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>
#include <iostream>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

struct ModelPt
{
	glm::vec3 position;
	glm::vec3 normal;
};

class VBO
{
public:
	mutable GLuint ID{};
	GLuint bufSize = 0;
	GLuint currentBufSize = 0;

	inline VBO();
	explicit inline VBO(const std::vector<Vertex>& vertices);
	explicit inline VBO(const std::vector<ModelPt>& vertices);
	explicit inline VBO(const std::vector<glm::vec3>& vertices);

	inline void PushData(const std::vector<glm::vec3>& vertices);
	inline void AllocBuffer(GLint size, GLenum type);

	inline void Bind() const;
	static inline void Unbind();
	inline void Delete() const;
};

// Constructors that generates a Vertex Buffer Object and links it to vertices
inline VBO::VBO()
{
	glGenBuffers(1, &ID);
}

inline VBO::VBO(const std::vector<glm::vec3>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(std::vector<glm::vec3>), vertices.data(), GL_STATIC_DRAW);
}

inline VBO::VBO(const std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

inline VBO::VBO(const std::vector<ModelPt>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ModelPt), vertices.data(), GL_STATIC_DRAW);
}

inline void VBO::PushData(const std::vector<glm::vec3>& vertices)
{
	if (currentBufSize + vertices.size() * sizeof(glm::vec3) > bufSize)
	{
		std::cout << "VBO Buffer Overflow..." << std::endl;
		return;
	}
	glBufferSubData(GL_ARRAY_BUFFER, currentBufSize, vertices.size() * sizeof(glm::vec3), vertices.data());
	currentBufSize += vertices.size() * sizeof(glm::vec3);
}

inline void VBO::AllocBuffer(const GLint size, const GLenum type)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, type);
	bufSize = size;
}

// Binds the VBO
inline void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
inline void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
inline void VBO::Delete() const
{
	glDeleteBuffers(1, &ID);
}