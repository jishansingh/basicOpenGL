#pragma once
#include"libs.h"
#include"Shader.h"

class Material {
private:
	glm::vec3 ambient;
	glm:: vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint specularTex;

public:
	Material(glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	GLint diffuseTex,
	GLint specularTex) {
		this->ambient=ambient;
		this->diffuse=diffuse;
		this->specular=specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}
	~Material(){}
	void sendToShader(Shader& prog) {
		prog.setUniform3f("material.ambient", GL_FALSE, this->ambient);
		prog.setUniform3f("material.diffuse", GL_FALSE, this->diffuse);
		prog.setUniform3f("material.specular", GL_FALSE, this->specular);

		prog.setUniform1i("material.diffuseTex", this->diffuseTex);
		prog.setUniform1i("material.specularTex", this->specularTex);

	}
	void sendToShader(Shader* prog) {
		prog->setUniform3f("material.ambient", GL_FALSE, this->ambient);
		prog->setUniform3f("material.diffuse", GL_FALSE, this->diffuse);
		prog->setUniform3f("material.specular", GL_FALSE, this->specular);

		prog->setUniform1i("material.diffuseTex", this->diffuseTex);
		prog->setUniform1i("material.specularTex", this->specularTex);

	}
};