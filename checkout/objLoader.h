#pragma once

#include"libs.h"
#include"Vertex.h"
#include<sstream>

static std::vector<Vertex> loadObj(const char* filename){
	std::vector<glm::fvec3>vertex_position;
	std::vector<glm::fvec3>vertex_normal;
	std::vector<glm::vec2>vertex_texcoord;


	std::vector<GLint> vertex_position_indices;
	std::vector<GLint> vertex_normal_indices;
	std::vector<GLint> vertex_texcoord_indices;

	std::vector<Vertex>vertices;

	std::stringstream ss;
	std::ifstream infile(filename);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	if (!infile.is_open()) {
		std::cout << "could not open file " << filename << std::endl;
		throw "ERROR";
	}
	while (std::getline(infile, line)) {
		ss.clear();
		ss.str(line);
		ss >> prefix;
		if (prefix == "#") {}
		else if (prefix == "o") {}
		else if (prefix == "s") {}
		else if (prefix == "use_mtl") {}
		else if (prefix == "v") {
			//vertex
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_position.push_back(temp_vec3);
		}
		else if (prefix == "vt") {
			//vertex
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoord.push_back(temp_vec2);
		}
		else if (prefix == "vn") {
			//vertex
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normal.push_back(temp_vec3);
		}
		else if (prefix == "f") {
			//vertex
			int counter = 0;
			while (ss >> temp_glint) {
				if (counter == 0)
					vertex_position_indices.push_back(temp_glint);
				else if (counter == 1)
					vertex_texcoord_indices.push_back(temp_glint);
				else if (counter == 2)
					vertex_normal_indices.push_back(temp_glint);
				
				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					++counter;
					ss.ignore(1, ' ');
				}
				if (counter > 1) {
					counter = 0;
				}
			}


		}
		else {}

		vertices.resize(vertex_position_indices.size(), Vertex());

		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].position = vertex_position[vertex_position_indices[i] - 1];
			vertices[i].texcoord = vertex_texcoord[vertex_texcoord_indices[i] - 1];
			vertices[i].normal = glm::vec3(0.f, 0.f, 1.f);
			vertices[i].color = glm::vec3(1.f, 0.f, 0.f);
		}
	}

	return vertices;
}

