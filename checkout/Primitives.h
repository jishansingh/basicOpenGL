#pragma once
#include"libs.h"
#include"Vertex.h"

class Primitive {
private:
	std::vector<Vertex2>vertices;
	std::vector<GLuint>indices;

public:
	Primitive() {

	}
	virtual ~Primitive() {

	}
	void set(Vertex2* vertexArray, int noOfVertices, GLuint* indexArray, int noOfIndices) {
		for (int i = 0; i < noOfVertices; i++) {
			vertices.push_back(vertexArray[i]);
		}
		for (int i = 0; i < noOfIndices; i++) {
			indices.push_back(indexArray[i]);
		}
	}

	inline Vertex2* getVertices() { return vertices.data(); }
	inline GLuint* getIndices() { return indices.data(); }

	inline const unsigned getNoOfVertices() { return vertices.size(); }
	inline const unsigned getNoOfIndices() { return indices.size(); }


};
/*
class Quad :public Primitive {
public:
	Quad() {

		Vertex vertices[] = {
			glm::vec3(-0.5f,0.5f,0.f)   ,glm::vec3(1.f,0.f,0.f) ,glm::vec2(0.f,1.f),glm::vec3(0.f,0.f,1.f),
			glm::vec3(-0.5f,-0.5f,0.0f) ,glm::vec3(0.f,1.f,0.f) ,glm::vec2(0.f,0.f),glm::vec3(0.f,0.f,1.f),
			glm::vec3(0.5f,-0.5f,0.0f)  ,glm::vec3(0.f,0.f,1.f) ,glm::vec2(1.f,0.f),glm::vec3(0.f,0.f,1.f),
			glm::vec3(0.5f,0.5f,0.0f)   ,glm::vec3(0.f,0.f,1.f) ,glm::vec2(1.f,1.f),glm::vec3(0.f,0.f,1.f),
		};


		unsigned int noOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0,1,2,
			0,2,3
		};
		unsigned int noOfIndex = sizeof(indices) / sizeof(GLuint);
		this->set(vertices, noOfVertices, indices, noOfIndex);

	}
};
*/
class Cube :public Primitive {
public:
	Cube() {

		Vertex2 vertices[] = {
			glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f),
			glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f),

			glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f),
			glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f),

			glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),

			glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f),
			glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),

			glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),

			glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f),
			glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f),
			glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)
		};


		unsigned int noOfVertices = sizeof(vertices) / sizeof(Vertex2);

		GLuint indices[1] = {};
		unsigned int noOfIndex = 0;
		this->set(vertices, noOfVertices, indices, noOfIndex);

	}
};

