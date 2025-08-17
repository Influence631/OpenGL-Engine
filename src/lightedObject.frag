#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D brick;
uniform sampler2D face;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;

float ambientStrength = 0.3f;

void main(){
	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(lightPos - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0f);
	
  vec3 diffuse = diff * lightColor;
	vec3 ambient = ambientStrength * lightColor;
  
	vec3 result = (ambient + diffuse) * objectColor;
	
	FragColor = vec4(result, 1.0f);
	//fragColor = mix(texture(brick, uv), texture(face, uv), 0.4f);
}
