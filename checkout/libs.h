#pragma once

#include<iostream>
#include<fstream>
#include<vector>
#include<string>

#include<glew.h>
#include<glfw3.h>

#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>


#include<SOIL2.h>

struct Vertex2 {
	glm::vec3 position;
	glm::vec2 texcoord;
};

