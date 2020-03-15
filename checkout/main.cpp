#include"libs.h"
#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"Vertex.h"
#include"Mesh.h"

void frameBufferResizeCallback(GLFWwindow*window,int fbW,int fbH) {
	glViewport(0, 0, fbW, fbH);
}


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

int main() {
	//init
	glfwInit();

	//create window
	int WINDOW_HEIGHT = 480;
	int WINDOW_WIDTH = 640;
	int framebufferwidth = 0;
	int framebufferheight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_WIDTH, "mywindow", NULL, NULL);

	glfwGetFramebufferSize(window, &framebufferwidth, &framebufferheight);
	//glViewport(0, 0, framebufferwidth, framebufferheight);

	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	glfwMakeContextCurrent(window);

	//init GLEW
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "error in glewinit";
		glfwTerminate();
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//create program

	Shader program("vertexShader.glsl", "fragmentShader.glsl","");

	/*GLuint program=glCreateProgram();
	if (!loadShaders(program)) {
		glfwTerminate();
	}*/
	
	Mesh test(&Quad());


	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//make vertices
	GLuint vb_id;
	glGenBuffers(1,&vb_id);
	glBindBuffer(GL_ARRAY_BUFFER, vb_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ibuf_id;
	glGenBuffers(1, &ibuf_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//tell gpu distribution of vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	//texture
	Texture texture0("images/pusheen.png", GL_TEXTURE_2D, 0);
	Texture texture1("images/container.png", GL_TEXTURE_2D, 1);

	Material material0(glm::vec3(0.1f),glm::vec3(1.f),glm::vec3(1.f),texture0.getTextureUnit(),texture1.getTextureUnit());
	

	//matrix operations
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);


	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f,0.f,0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::scale(modelMatrix, scale);

	glm::vec3 camPosition(0.f,0.f,1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);

	glm::mat4 viewMatrix(1.f);
	viewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 100.f;
	glm::mat4 projectionMatrix(1.f);
	projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferwidth) / framebufferheight, nearPlane, farPlane);


	program.Use();
	//lightening
	glm::vec3 lightPos0(0.f, 0.f, 1.f);
	program.setUniform3f("lightPos0", GL_FALSE, lightPos0);
	program.setUniform3f("cameraPos", GL_FALSE, camPosition);
	
	program.setUniformMatrix4fv("modelMatrix",GL_FALSE, modelMatrix);
	program.setUniformMatrix4fv("viewMatrix", GL_FALSE, viewMatrix);
	program.setUniformMatrix4fv("projectionMatrix", GL_FALSE, projectionMatrix);


	program.unUse();
	//init main loop
	while (!glfwWindowShouldClose(window)) {
		//update input
		glfwPollEvents();
		//update
		updateInput(window);
		updateInput(window, test);
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

	}
	glfwDestroyWindow(window);

	glfwTerminate();


	return 0;
}


