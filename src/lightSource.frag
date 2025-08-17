#version 460 core
out vec4 FragColor;

//this will be a "const" color for the light source
uniform vec3 lightColor = vec3(0.26f, 0.7f, 0.3f);

void main(){
	FragColor = vec4(lightColor, 1.0f);
}
