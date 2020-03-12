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
	glm::vec3(0.0f,0.5f,0.5f) ,glm::vec3(1.f,0.f,0.f) ,glm::vec2(0.f,1.f),
	glm::vec3(-0.5f,-0.5f,0.0f) ,glm::vec3(0.f,1.f,0.f) ,glm::vec2(0.f,0.f),
	glm::vec3(0.5f,-0.5f,0.0f) ,glm::vec3(0.f,0.f,1.f) ,glm::vec2(1.f,0.f),
};


unsigned int noOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
	0,1,2
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

	//glfwGetFramebufferSize(window, &framebufferwidth, &framebufferheight);
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


	//init main loop
	while (!glfwWindowShouldClose(window)) {
		//update input
		glfwPollEvents();
		//update
		updateInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(program);

		//bind vao
		glBindVertexArray(vao);

		//draw
		glDrawElements(GL_TRIANGLES, noOfIndex, GL_UNSIGNED_INT, 0);


		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glfwSwapBuffers(window);
		glFlush();
	}
	glfwDestroyWindow(window);

	glfwTerminate();

	glDeleteProgram(program);

	return 0;
}


