#pragma once


#include"libs.h"
#include"Vertex.h"
#include"Shader.h"

class Mesh {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;
	//functions
	void initVertexData(Vertex* vertexArray,int noOfVertices,GLuint* indicesArray,int noOfIndices) {
		for (int i = 0; i < noOfVertices; i++) {
			vertices.push_back(vertexArray[i]);
		}
		for (int i = 0; i < noOfIndices; i++) {
			indices.push_back(indicesArray[i]);
		}
	}
	void initVAO() {
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//make vertices
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		//tell gpu distribution of vertex
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);
	}
	void initModelMatrix() {

		this->modelMatrix=glm::mat4(1.f);
		this->modelMatrix = glm::translate(this->modelMatrix, this->position);
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->modelMatrix = glm::scale(this->modelMatrix, scale);
	}
	void updateUniform(Shader* shader) {
		shader->setUniformMatrix4fv("modelMatrix", GL_FALSE, this->modelMatrix);
	}
	void updateInput(GLFWwindow *window) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position.z += 0.003f;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position.z -= 0.003f;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			rotation.y += 0.01f;
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			rotation.y -= 0.01f;
		}
	}
public:
	Mesh(Vertex* vertexArray, int noOfVertices, GLuint* indicesArray, int noOfIndices) {
		this->position = glm::vec3(0.f);
		this->rotation = glm::vec3(0.f);
		this->scale = glm::vec3(1.f);

		this->initVertexData(vertexArray, noOfVertices, indicesArray, noOfIndices);
		this->initVAO();
		this->initModelMatrix();
	}
	~Mesh(){
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, & this->ibo);
	}
	void update() {

	}
	void render(GLFWwindow* window,Shader* shader) {
		//this->updateInput(window);
		this->initModelMatrix();
		this->updateUniform(shader);

		//bind vao
		glBindVertexArray(this->vao);

		//draw
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	}

};
