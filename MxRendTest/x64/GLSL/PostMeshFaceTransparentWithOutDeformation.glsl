#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 planes[8];
//float gl_ClipDistance[8];

//光照部分
out vec3 FragPos;
out vec3 Normal;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);

	for(int i = 0;i < 8; ++i)
	{
		gl_ClipDistance[i] = dot(planes[i],vec4(aPos, 1.0f));	
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

uniform int lightIsOn;

uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 normal, vec3 matColor)
{
	if(lightIsOn == 0)
	{
		return matColor;
	}
	//ambient
	vec3 ambient = light.ambient * matColor;
		
	// diffuse 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * matColor);

	// specular
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir); 
	float spec = pow(max(dot(normal, halfwayDir), 0.0), light.shiness);
    vec3 specular = light.specular * (spec * 0.3);  

	vec3 result = ambient + diffuse + specular;
	return result;
	
}

void main()
{
	vec3 normal;
	if( gl_FrontFacing ) 
	{
		normal = Normal;
	}
	else
	{
		normal = -Normal;
	}
	
	vec3 color = vec3(1,1,1);
	FragColor = vec4(calculateLightResult(normal,color), 0.3);
} 

#endif


