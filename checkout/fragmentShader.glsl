#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcords;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 lightPos0;
uniform vec3 cameraPos;
void main(){
	//fs_color=vec4(vs_color,1.0f);
	//ambient light
	vec3 ambientLight=vec3(0.1f,0.1f,0.1f);

	vec3 posToLight=normalize(vs_position-lightPos0);
	vec3 diffuseColor=vec3(1.f,1.f,1.f);
	float diffuse=clamp(dot(posToLight,vs_normal),0,1);
	vec3 diffuse_final=diffuse*diffuseColor;
	

	//specular

	vec3 lightToPosVec=normalize(lightPos0-vs_position);
	vec3 relectVec=normalize(reflect(lightToPosVec,normalize(vs_normal)));
	vec3 posToView=normalize(vs_position-cameraPos);
	float specularConst=pow(max(dot(posToView,relectVec),0),30);
	vec3 specularFinal=vec3(1.f)*specularConst;

	fs_color=texture(texture0,vs_texcords)*vec4(vs_color,1.0f)*(vec4(ambientLight,1.f)+vec4(diffuse_final,1.f)+vec4(specularFinal,1.f));

	//*texture(texture1,vs_texcords)*vec4(vs_color,1.0f);
}
