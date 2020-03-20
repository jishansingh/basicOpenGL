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
	glCullFace(GL_BACK);*/
	
	glFrontFace(GL_CCW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	objShader.push_back(new Shader("vertexShader.glsl", "fragmentShader.glsl", ""));
}
void Game::initTextures()
{
	objTexture.push_back(std::make_pair(new Texture("images/container.png", GL_TEXTURE_2D, 0), new Texture("images/container_specular.png", GL_TEXTURE_2D, 1)));
}
void Game::initMaterials()
{
	objMaterial.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), objTexture[0].first->getTextureUnit(), objTexture[0].second->getTextureUnit()));
}
void Game::initMesh()
{
	std::vector<Vertex>mesh = loadObj("objFile/Crate1.obj");

	objMesh.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0,
		glm::vec3(2.f, 0.f, 0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)));
	objMesh.push_back(new Mesh(&Pyramid(),glm::vec3(0.f),glm::vec3(0.f),glm::vec3(1.f)));
	objMesh.push_back(new Mesh(&Quad(), glm::vec3(1.f), glm::vec3(0.f), glm::vec3(1.f)));
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
void Game::initModel()
{
	models.push_back(new Model(glm::vec3(0.f), this->objMaterial[0], this->objTexture[0].first, this->objTexture[0].second, objMesh));
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
	this->initModel();
	this->initUniform();

	//intialize delta time
	dt = 0.f;
	curTime = 0.f;
	lastTime = 0.f;

	//mouse Positions
	lastMouseX = 0.0;
	lastMouseY = 0.0;
	mouseX = 0.0;
	mouseY = 0.0;
	mouseOffsetX = 0.0;
	mouseOffsetY = 0.0;
	firstMouse = true;

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
	for (int i = 0; i < objMesh.size(); i++) {
		delete objMesh[i];
	}
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}

}



void Game::updateKeyboardInput() {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos.z -= 0.01f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos.z += 0.01f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos.x -= 0.01f;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos.x += 0.01f;
	}
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(window, &this->mouseX, &this->mouseY);
	if (this->firstMouse) {
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->mouseY - this->lastMouseY;
	cameraPos.x += 0.01f * this->mouseOffsetX*dt;
	cameraPos.y += 0.01f * this->mouseOffsetY*dt;
}

void Game::updateUniform()
{
	//update projection matrix
	glfwGetFramebufferSize(window, &framebufferwidth, &framebufferheight);
	projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferwidth) / framebufferheight, nearPlane, farPlane);
	this->objShader[0]->setUniformMatrix4fv("projectionMatrix", GL_FALSE, projectionMatrix);

	//update View matrix
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
	this->objShader[0]->setUniformMatrix4fv("viewMatrix", GL_FALSE, viewMatrix);
}

void Game::updateDT()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}


void Game::update()
{
	glfwPollEvents();
	this->updateDT();
	this->updateKeyboardInput();
	this->updateMouseInput();
}

void Game::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->objShader[0]->Use();
	
	this->updateUniform();

	
	this->models[0]->render(this->objShader[0]);

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glfwSwapBuffers(this->window);
	glFlush();

	this->objShader[0]->unUse();
}
void Game::frameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}
