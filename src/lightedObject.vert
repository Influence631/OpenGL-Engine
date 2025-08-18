#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	uv = aUV;

	//convert the normals from local to world coords (and getting rid of translation part of the matrix (by casting to 3x3 mat) since normals dont have a homogenous w coord).
	//calculating an inverse is expensive so it shouldnt really happen in the vertex shader, rather calculate the normal matrix on the cpu and set using a uniform
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));
}
