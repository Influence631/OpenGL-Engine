#version 460 core
out vec4 FragColor;

//in vec2 uv;

uniform sampler2D brick;
uniform sampler2D face;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
	FragColor = vec4(lightColor * objectColor, 1.0f);
	//fragColor = mix(texture(brick, uv), texture(face, uv), 0.4f);
}
