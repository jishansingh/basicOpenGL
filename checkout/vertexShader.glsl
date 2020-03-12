#version 440

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec3 vertex_color;
layout (location=2) in vec2 vertex_texcords;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcords;

void main(){
	vs_position=vertex_position;
	vs_color=vertex_color;
	vs_texcords=vertex_texcords;
	gl_Position=vec4(vertex_position,1.f);
}
