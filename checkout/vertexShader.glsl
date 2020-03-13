#version 440

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec3 vertex_color;
layout (location=2) in vec2 vertex_texcords;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	vs_position=vec4(modelMatrix*vec4(vertex_position,1.f)).xyz;
	vs_color=vertex_color;
	vs_texcords=vec2(vertex_texcords.x,vertex_texcords.y*(-1.f));
	gl_Position=projectionMatrix*viewMatrix*modelMatrix*vec4(vertex_position,1.f);
}
