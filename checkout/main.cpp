#include"libs.h"
#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"Vertex.h"
#include"Mesh.h"
#include"Game.h"

void updateInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

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

void updateInput(GLFWwindow* window,Mesh & mesh) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.f,0.f,0.003f));
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.f, 0.f, -0.003f));
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(0.f, 0.01f, 0.f));
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(0.f, -0.01f, 0.f));
	}
}
GLFWwindow* createWindow(const char* window_title,int width, int height, int glMajorVer, int glMinorVer, bool resizable, int& fbwidth, int fbheight) {
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVer);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	GLFWwindow* window = glfwCreateWindow(width, width, window_title, NULL, NULL);

	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	//glViewport(0, 0, framebufferwidth, framebufferheight);

	//glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	glfwMakeContextCurrent(window);
	return window;
}
int main() {
	//create window
	int WINDOW_HEIGHT = 480;
	int WINDOW_WIDTH = 640;
	int framebufferwidth = 0;
	int framebufferheight = 0;
	Game game("mywindow", WINDOW_WIDTH, WINDOW_HEIGHT, 4, 4, true);


	
	
	//init main loop
	while (!game.getWindowShouldClose()) {
		//update input
		game.update();
		//update
		//updateInput(window);
		//updateInput(window, test);
		/*
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		program.Use();

		

		glm::mat4 modelMatrix(1.f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		modelMatrix = glm::scale(modelMatrix, scale);

		program.setUniformMatrix4fv("modelMatrix", GL_FALSE, modelMatrix);
		
		glfwGetFramebufferSize(window, &framebufferwidth, &framebufferheight);
		projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferwidth) / framebufferheight, nearPlane, farPlane);
		
		program.setUniformMatrix4fv("projectionMatrix", GL_FALSE, projectionMatrix);

		texture0.bind();
		texture1.bind();
		material0.sendToShader(program);
		//bind vao
		glBindVertexArray(vao);

		//draw
		glDrawElements(GL_TRIANGLES, noOfIndex, GL_UNSIGNED_INT, 0);
		test.render(&program);

		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		program.unUse();
		*/
		game.render();
	}


	return 0;
}


