#include "Game.h"



void Game::initialize(const char* window_title, bool resizable)
{
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVer);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title, NULL,NULL);
	if (this->window == nullptr) {
		std::cout << "window creation failed" << std::endl;
		glfwTerminate();
	}
	glfwGetFramebufferSize(window, &framebufferwidth, &framebufferheight);
	//glViewport(0, 0, framebufferwidth, framebufferheight);

	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	glfwMakeContextCurrent(window);
}

void Game::initGLFW()
{
	if (glfwInit()==GL_FALSE) {
		std::cout << "init glfw failed" << std::endl;
		glfwTerminate();
	}
}

void Game::initGLEW()
{
	//init GLEW
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "error in glewinit"<<std::endl;
		glfwTerminate();
	}
}

void Game::initOpenGLOption()
{
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/
	glFrontFace(GL_CCW);
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(window,GLFW_TRUE);
}
void Game::initViewMatrix() {
	cameraPos = glm::vec3(0.f, 0.f, 1.f);
	worldUp=glm::vec3(0.f, 1.f, 0.f);
	cameraFront = glm::vec3(0.f, 0.f, -1.f);

	viewMatrix = glm::mat4(1.f);
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
}
void Game::initProjection()
{
	fov = 90.f;
	nearPlane = 0.1f;
	farPlane = 100.f;
	projectionMatrix = glm::mat4(1.f);
	projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferwidth) / framebufferheight, nearPlane, farPlane);

}
void Game::initShaders()
{
	objShader.push_back(new Shader("3DVertexShader.glsl", "3DFragmentshader.glsl", ""));
}
void Game::initTextures()
{
	objTexture.push_back(std::make_pair(new Texture("images/container.png", GL_TEXTURE_2D, 0), new Texture("images/container_specular.png", GL_TEXTURE_2D, 1)));
}
void Game::initMaterials()
{
	objMaterial.push_back(new Material(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(1.f), objTexture[0].first->getTextureUnit(), objTexture[0].second->getTextureUnit()));
}
void Game::initMesh()
{
	objMesh.push_back(new Mesh(&Cube()));
}
void Game::initLight()
{
	lightPos.push_back(glm::vec3(0.f, 0.f, 1.f));
}
void Game::initUniform()
{
	this->objShader[0]->Use();
	this->objShader[0]->setUniform3f("lightPos0", GL_FALSE, lightPos[0]);
	this->objShader[0]->setUniform3f("cameraPos", GL_FALSE, cameraPos);

	this->objShader[0]->setUniformMatrix4fv("viewMatrix", GL_FALSE, viewMatrix);
	this->objShader[0]->setUniformMatrix4fv("projectionMatrix", GL_FALSE, projectionMatrix);
}
Game::Game(const char* window_title, int width, int height, int gl_major_ver, int gl_minor_ver, bool resizable):
	WINDOW_WIDTH(width),WINDOW_HEIGHT(height),glMinorVer(gl_minor_ver), glMajorVer(gl_major_ver),framebufferwidth(width),framebufferheight(height)
{

	this->initGLFW();
	this->initialize(window_title,resizable);
	//matrix initialization
	this->initViewMatrix();
	this->initProjection();

	this->initGLEW();
	this->initOpenGLOption();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initMesh();
	this->initLight();
	this->initUniform();
	
	

}

Game::~Game()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	for (int i = 0; i < objShader.size(); i++) {
		delete objShader[i];
	}
	for (int i = 0; i < objTexture.size(); i++) {
		delete objTexture[i].first;
		delete objTexture[i].second;
	}
	for (int i = 0; i < objMaterial.size(); i++) {
		delete objMaterial[i];
	}
	for (int i = 0; i < objMesh.size(); i++) {
		delete objMesh[i];
	}
}

void Game::update()
{
	glfwPollEvents();
}

void Game::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	this->objShader[0]->Use();


	glfwGetFramebufferSize(window, &framebufferwidth, &framebufferheight);
	projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferwidth) / framebufferheight, nearPlane, farPlane);

	this->objShader[0]->setUniformMatrix4fv("projectionMatrix", GL_FALSE, projectionMatrix);
	
	

	objTexture[0].first->bind();
	objTexture[0].second->bind();
	this->objMaterial[0]->sendToShader(this->objShader[0]);
	//bind vao
	//glBindVertexArray(vao);

	//draw
	//glDrawElements(GL_TRIANGLES, noOfIndex, GL_UNSIGNED_INT, 0);
	this->objMesh[0]->render(this->objShader[0],window);

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glfwSwapBuffers(this->window);
	glFlush();

	glBindVertexArray(0);
	this->objShader[0]->unUse();
}
void Game::frameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}
