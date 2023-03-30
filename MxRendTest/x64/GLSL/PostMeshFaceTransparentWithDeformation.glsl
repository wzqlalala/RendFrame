#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aMaterial;
layout (location = 2) in float aIsColor;
layout (location = 3) in float aValue;
layout (location = 4) in vec3 aDisplacement;
layout (location = 5) in float aHasValue;
layout (location = 6) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 deformationScale;

uniform vec4 planes[8];

flat out int isColor;

//光照部分
out vec3 FragPos;
out vec3 Normal;

void main()
{
	vec3 deformationPos = aPos + deformationScale * aDisplacement;
	FragPos = vec3(model * vec4(deformationPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
	
	isColor = int(aIsColor);
	for(int i = 0;i < 8; ++i){
		gl_ClipDistance[i] = dot(planes[i],vec4(deformationPos, 1.0f));	
	}
	
	Normal = mat3(transpose(inverse(model))) * aNormal; 
}
#endif


#ifdef fragment_shader

out vec4 FragColor;
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

in vec3 FragPos;  
in vec3 Normal; 
flat in int isColor;

uniform int lightIsOn;
uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 normal, vec3 matColor)
{
	if(lightIsOn == 0) return matColor;

	vec3 ambient = light.ambient * matColor;		//ambient
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * matColor);	// diffuse 
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir); 
	float spec = pow(max(dot(normal, halfwayDir), 0.0), light.shiness);
    vec3 specular = light.specular * (spec * 0.3); 	// specular 

	return ambient + diffuse + specular;
}

void main()
{
	vec3 normal = gl_FrontFacing ? Normal : -Normal;
	vec3 color = vec3(1.0);
	FragColor = vec4(calculateLightResult(normal,color), 0.3);
} 

#endif


