#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;

	float shininess;
};

struct DirectionalLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;

	float innerCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform bool useDirectional;
uniform bool useSpot;

uniform sampler2D face;
uniform vec3 viewPos;

const int nrPointLights = 4;
uniform int nrActivePointLights;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[nrPointLights];
uniform SpotLight spotLight;

uniform Material material;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 diffColor, vec3 specColor, vec3 norm, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 diffColor, vec3 specColor, vec3 norm, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 diffColor, vec3 specColor, vec3 norm, vec3 viewDir);

void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 diffuseColor = texture(material.texture_diffuse1, uv).rgb;
	vec3 specColor = texture(material.texture_specular1, uv).rgb;
	
	vec3 result = vec3(0.0f);
	result += calculateDirectionalLight(directionalLight, diffuseColor, specColor, norm, viewDir);

	for(int i = 0; i < nrActivePointLights; i++){
			result += calculatePointLight(pointLights[i], diffuseColor, specColor, norm, viewDir);
	}
	
	result += calculateSpotLight(spotLight, diffuseColor, specColor, norm, viewDir);

	FragColor = vec4(result, 1.0f);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 diffColor, vec3 specColor, vec3 norm, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	vec3 ambient = light.ambient * diffColor;
	
	//the view dir has to be from the fragpos
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * diffColor;
	
	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess); 
	vec3 specular = light.specular * spec * specColor;

	return ambient + diffuse + specular;
}
vec3 calculatePointLight(PointLight light, vec3 diffColor, vec3 specColor, vec3 norm, vec3 viewDir){
	vec3 ambient = light.ambient * diffColor;
	
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * diffColor;

	vec3 reflectDir = normalize(reflect(-lightDir, norm)); 
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * specColor;
	
	float dist = length(light.position - FragPos);
	float attenuation = (1.0f / (light.constant + dist * light.linear + dist * dist * light.quadratic)); 
	return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 diffColor, vec3 specColor, vec3 norm, vec3 viewDir){
		vec3 ambient = light.ambient * diffColor;
		
		vec3 lightDir = normalize(FragPos - light.position);
		float diff = max(dot(norm, -lightDir), 0.0f);
		vec3 diffuse = light.diffuse * diff * diffColor;
	
		vec3 reflectDir = normalize(reflect(lightDir, norm)); 
		float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
		vec3 specular = light.specular * spec * specColor;
		
		//spotlight(softedges);	
		float theta = dot(lightDir, normalize(light.direction));
		float epsilon = light.innerCutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
		
		//dont affect ambient so we always have a bit of color even in dark;
		diffuse *= intensity;
		specular *= intensity;
			
		float dist = length(light.position - FragPos);
		float attenuation = 1.0f / (light.constant + dist * light.linear + dist * dist * light.quadratic); 
		
		vec3 result = (ambient + diffuse + specular) * attenuation;
		return result;
}
