#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

struct Material{
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

struct Light{
	//vec3 position;
	vec4 lightVector;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D face;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;


// can optionally make the ambient add global illumination
void main(){
	
	//diffuse lighting	
	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(light.lightVector.xyz - FragPos);
	if(light.lightVector.w == 0.0)
		lightDir = vec3(normalize(-light.lightVector));
	float diff = max(dot(norm, lightDir), 0.0f);
  
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, uv));
  
	//ambient light, depends on the diffuse lighting and comes from the same diffuse map
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uv));
	
	//specular lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);	
	vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));		

	vec3 result = ambient + diffuse + specular;
	
	FragColor = vec4(result, 1.0f);
	//fragColor = mix(texture(brick, uv), texture(face, uv), 0.4f);
}
