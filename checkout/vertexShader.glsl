#version 440

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec3 vertex_color;
layout (location=2) in vec2 vertex_texcords;
layout (location=3) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcords;
out vec3 vs_normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	vs_normal=mat3(modelMatrix)*vertex_normal;
	vs_position=vec4(modelMatrix*vec4(vertex_position,1.f)).xyz;
	vs_color=vertex_color;
	vs_texcords=vec2(vertex_texcords.x,vertex_texcords.y*(-1.f));
	gl_Position=projectionMatrix*viewMatrix*modelMatrix*vec4(vertex_position,1.f);
}
