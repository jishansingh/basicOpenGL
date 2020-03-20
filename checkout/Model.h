#pragma once

#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"

class Model {
private:
	Material* material;
	Texture* diffuseTex;
	Texture* specularTex;
	std::vector<Mesh*>meshes;
	glm::vec3 position;
public:
	Model(glm::vec3 position,Material*material,Texture* diffTexture,Texture* specTexture,std::vector<Mesh*>mesh_vec) {
		this->position = position;
		this->material = material;
		this->diffuseTex = diffTexture;
		this->specularTex = specTexture;

		for (auto* i : mesh_vec) {
			this->meshes.push_back(new Mesh(*i));
		}
	}
	~Model() {
		for (auto*& i : meshes) {
			delete i;
		}
	}
	void update() {

	}
	void render(Shader* shader) {
		shader->Use();

		diffuseTex->bind();
		specularTex->bind();
		material->sendToShader(shader);
		//bind vao
		//glBindVertexArray(vao);

		//draw
		//glDrawElements(GL_TRIANGLES, noOfIndex, GL_UNSIGNED_INT, 0);
		for (auto& i : this->meshes) {
			i->render(shader);
		}
		

	}
};



