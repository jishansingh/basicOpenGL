#pragma once
#include"libs.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"
#include"Mesh.h"
#include"Primitives.h"
class Game
{
private:
	//variables
	GLFWwindow* window;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int framebufferwidth;
	int framebufferheight;

	int glMajorVer;
	int glMinorVer;
	//view matrix
	glm::mat4 viewMatrix;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 worldUp;

	//projection Matrix
	glm::mat4 projectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;
	
	//object properties and looks
	std::vector<Shader*>objShader;
	std::vector<std::pair<Texture*, Texture*> >objTexture;
	std::vector<Material*>objMaterial;
	std::vector<Mesh*>objMesh;
	std::vector<glm::vec3>lightPos;

	//functions
	void initialize(const char* window_title, bool resizable);
	void initGLFW();
	void initGLEW();
	void initOpenGLOption();

	void initViewMatrix();
	void initProjection();

	//init shaders textures material
	void initShaders();
	void initTextures();
	void initMaterials();
	//init mesh light
	void initMesh();
	void initLight();
	//init uniforms
	void initUniform();

	//static variables
	
public:
	Game(const char* window_title, int width, int height, int gl_major_ver, int gl_minor_ver, bool resizable);
	virtual ~Game();

	int getWindowShouldClose();
	void setWindowShouldClose();
	void update();
	void render();
	static void frameBufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
};

