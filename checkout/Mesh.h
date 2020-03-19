#pragma once


#include"libs.h"
#include"Vertex.h"
#include"Shader.h"
#include"Primitives.h"
class Mesh {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	int noOfVertices;
	int noOfIndices;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;
	//functions
	void initVAO(Vertex* vertices, int noOfVertices, GLuint* indices, int noOfIndex) {
		this->noOfVertices = noOfVertices;
		this->noOfIndices = noOfIndex;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//make vertices
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, noOfVertices*sizeof(Vertex2), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, noOfIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);

		//tell gpu distribution of vertex
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, texcoord));
		glEnableVertexAttribArray(2);

		/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);*/
		

		glBindVertexArray(0);
	}
	void initVAO(Primitive* primitive) {
		this->noOfVertices = primitive->getNoOfVertices();
		this->noOfIndices = primitive->getNoOfIndices();
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//make vertices
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, noOfVertices * sizeof(Vertex2), primitive->getVertices(), GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, noOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

		//tell gpu distribution of vertex
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, texcoord));
		glEnableVertexAttribArray(1);

		/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);*/

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
	void updateInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			this->move(glm::vec3(0.f, 0.f, 0.1f));
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			this->move(glm::vec3(0.f, 0.f, -0.1f));
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			this->rotate(glm::vec3(0.f, 0.01f, 0.f));
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			this->rotate(glm::vec3(0.f, -0.01f, 0.f));
		}
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			this->scaleUp(glm::vec3(1.5f));
		}
		else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			this->scaleUp(glm::vec3(0.5f));
		}
	}
public:
	Mesh(Vertex* vertexArray,int noOfVertices, GLuint* indicesArray,int noOfIndex) {
		this->position = glm::vec3(0.f);
		this->rotation = glm::vec3(0.f);
		this->scale = glm::vec3(1.f);

		this->initVAO(vertexArray,noOfVertices, indicesArray,noOfIndex);
		this->initModelMatrix();
	}
	Mesh(Primitive* primitive) {
		this->position = glm::vec3(0.f);
		this->rotation = glm::vec3(0.f);
		this->scale = glm::vec3(1.f);

		this->initVAO(primitive);
		this->initModelMatrix();
	}
	~Mesh(){
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, & this->ibo);
	}
	void update() {

	}
	void render(Shader* shader,GLFWwindow*window) {
		this->updateInput(window);
		this->initModelMatrix();
		this->updateUniform(shader);
		shader->Use();
		//bind vao
		glBindVertexArray(this->vao);

		//draw
		if (noOfIndices == 0) {
			glDrawArrays(GL_TRIANGLES,0, noOfVertices);
			return;
		}
		glDrawElements(GL_TRIANGLES, noOfIndices, GL_UNSIGNED_INT, 0);
	}
	void move(glm::vec3 pos) {
		this->position += pos;
	}
	void rotate(glm::vec3 rot) {
		this->rotation += rot;
	}
	void scaleUp(glm::vec3 times) {
		this->scale += times;
	}


};
