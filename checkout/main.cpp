#include"libs.h"

void frameBufferResizeCallback(GLFWwindow*window,int fbW,int fbH) {
	glViewport(0, 0, fbW, fbH);
}

bool checkErrors(GLuint shader_id) {
	char infoLog[512];
	GLint success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
		std::cout << "ERROR: LOADING SHADER COULD NOT COMPILE SHADER" << std::endl;
		std::cout << infoLog << std::endl;
		return false;
	}
	return true;
}


std::string extractSource(const char* filename) {
	std::string shaderSource = "";
	std::string temp = "";
	std::ifstream fil_index;
	fil_index.open(filename);
	if (fil_index.is_open()) {
		while (std::getline(fil_index, temp))
			shaderSource += temp + "\n";
	}
	else {
		std::cout << "UNABLE TO OPEN FILE " << filename << std::endl;
		return "";
	}
	fil_index.close();
	return shaderSource;
}

bool loadShaders(GLuint prog_id) {
	bool success = true;

	std::string vertexSource = extractSource("vertexShader.glsl");
	GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexSrc = vertexSource.c_str();
	glShaderSource(vertex_id, 1, &vertexSrc, NULL);
	glCompileShader(vertex_id);

	success &= checkErrors(vertex_id);

	std::string fragmentSource = extractSource("fragmentShader.glsl");
	const GLchar* fragmentSrc = fragmentSource.c_str();
	GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &fragmentSrc, NULL);
	glCompileShader(fragment_id);
	success &= checkErrors(fragment_id);

	//attach to prog_id
	glAttachShader(prog_id, vertex_id);
	glAttachShader(prog_id, fragment_id);

	glLinkProgram(prog_id);

	glUseProgram(0);
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
	return success;
}

void updateInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

Vertex vertices[] = {
	glm::vec3(-0.5f,0.5f,0.f) ,glm::vec3(1.f,0.f,0.f) ,glm::vec2(0.f,1.f),
	glm::vec3(-0.5f,-0.5f,0.0f) ,glm::vec3(0.f,1.f,0.f) ,glm::vec2(0.f,0.f),
	glm::vec3(0.5f,-0.5f,0.0f) ,glm::vec3(0.f,0.f,1.f) ,glm::vec2(1.f,0.f),
	glm::vec3(0.5f,0.5f,0.0f) ,glm::vec3(0.f,0.f,1.f) ,glm::vec2(1.f,1.f),
};


unsigned int noOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
	0,1,2,
	0,2,3
};
unsigned int noOfIndex = sizeof(indices) / sizeof(GLuint);

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
	GLuint program=glCreateProgram();
	if (!loadShaders(program)) {
		glfwTerminate();
	}

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
	glBindVertexArray(0);

	//texture
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image("images/pusheen.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);
	


	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE LOADING FAILED" << "\n";
	}
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D,0);

	SOIL_free_image_data(image);



	//texture2
	int image_width1 = 0;
	int image_height1 = 0;
	unsigned char* image1 = SOIL_load_image("images/container.png", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE LOADING FAILED" << "\n";
	}
	glActiveTexture(1);
	glBindTexture(GL_TEXTURE_2D, 1);

	SOIL_free_image_data(image1);

	//matrix operations
	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f));
	modelMatrix = glm::rotate(modelMatrix,glm::radians(0.f), glm::vec3(1.f,0.f,0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f));

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


	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	glUseProgram(0);
	//init main loop
	while (!glfwWindowShouldClose(window)) {
		//update input
		glfwPollEvents();
		//update
		updateInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(program);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f));

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glfwGetFramebufferSize(window, &framebufferwidth, &framebufferheight);
		projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferwidth) / framebufferheight, nearPlane, farPlane);
		glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		
		glUniform1i(glGetUniformLocation(program, "texture0"), 0);

		glUniform1i(glGetUniformLocation(program, "texture1"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//bind vao
		glBindVertexArray(vao);

		//draw
		glDrawElements(GL_TRIANGLES, noOfIndex, GL_UNSIGNED_INT, 0);


		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);

	}
	glfwDestroyWindow(window);

	glfwTerminate();

	glDeleteProgram(program);

	return 0;
}


