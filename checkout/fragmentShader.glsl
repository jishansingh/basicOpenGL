#version 440

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};


in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcords;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform vec3 lightPos0;
uniform vec3 cameraPos;

//functions

vec4 calcAmbient(Material mat){
	return vec4(mat.ambient,1.f);
}

vec4 calcDiffuse(Material mat,vec3 vs_position,vec3 lightPos,vec3 normal){
	vec3 posToLight=normalize(lightPos-vs_position);
	vec3 diffuseColor=material.diffuse;
	float diffuse=clamp(dot(posToLight,normal),0,1);
	vec3 diffuse_final=diffuse*diffuseColor;
	return vec4(diffuse_final,1.f);
}

vec4 specFinal(Material mat,vec3 vs_position,vec3 lightPos,vec3 normal,vec3 cameraPosition){
	vec3 lightToPosVec=normalize(vs_position-lightPos);
	vec3 relectVec=normalize(reflect(lightToPosVec,normalize(normal)));
	vec3 posToView=normalize(cameraPosition-vs_position);
	float specularConst=pow(max(dot(posToView,relectVec),0),30);
	vec3 specularFinal=material.specular*specularConst;
	return vec4(specularFinal,1.f);
}
void main(){
	//fs_color=vec4(vs_color,1.0f);
	//ambient light
	vec4 ambientFinal=calcAmbient(material);

	vec4 diffuseFinal=calcDiffuse(material,vs_position,lightPos0,vs_normal);
	
	//specular

	vec4 specFinal=specFinal(material,vs_position,lightPos0,vs_normal,cameraPos);


	fs_color=texture(material.diffuseTex,vs_texcords)*texture(material.specularTex,vs_texcords)*vec4(vs_color,1.0f)
		*(ambientFinal+diffuseFinal+specFinal);

	//*texture(texture1,vs_texcords)*vec4(vs_color,1.0f);
}
