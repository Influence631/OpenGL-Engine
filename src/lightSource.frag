#version 460 core
out vec4 FragColor;

//this will be a "const" color for the light source
uniform vec3 lightColor = vec3(0.1f, 0.9f, 0.3f);

void main(){
	FragColor = vec4(lightColor, 1.0f);
}
