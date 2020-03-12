#include"libs.h"

void frameBufferResizeCallback(GLFWwindow*window,int fbW,int fbH) {
	glViewport(0, 0, fbW, fbH);
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

	//init main loop
	while (!glfwWindowShouldClose(window)) {
		//update input
		glfwPollEvents();
		//update

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glfwSwapBuffers(window);
		glFlush();
	}
	glfwTerminate();
	return 0;
}


