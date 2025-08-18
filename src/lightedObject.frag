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
uniform vec3 viewPos;

float ambientStrength = 0.3f;
float specularStrength = 0.5f;
float shininess = 128;

//implementing Phong lighting
void main(){
	
	//ambient light
	vec3 ambient = ambientStrength * lightColor;
	
	//diffuse lighting	
	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = diff * lightColor;
  
	//specular lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);	
	vec3 specular = specularStrength * spec * lightColor;		

	vec3 result = (ambient + diffuse + specular) * objectColor;
	
	FragColor = vec4(result, 1.0f);
	//fragColor = mix(texture(brick, uv), texture(face, uv), 0.4f);
}
