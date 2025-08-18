#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform sampler2D brick;
uniform sampler2D face;

uniform Material material;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.4f;
float specularStrength = 0.5f;

//implementing Phong lighting
void main(){
	
	//ambient light
	vec3 ambient = material.ambient * ambientStrength + lightColor * material.ambient;
	
	//diffuse lighting	
	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = lightColor * diff * material.diffuse;
  
	//specular lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);	
	vec3 specular = lightColor * spec * material.specular;		

	vec3 result = ambient + diffuse + specular;
	
	FragColor = vec4(result, 1.0f);
	//fragColor = mix(texture(brick, uv), texture(face, uv), 0.4f);
}
