#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aPlaneIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 planes[8];
//float gl_ClipDistance[8];

out float TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord  = aTexCoord;
	int planeIndex =  int(aPlaneIndex);
	for(int i = 0;i < 8; ++i)
	{
		if(planeIndex != i)
		{
			gl_ClipDistance[i] = dot(planes[i],vec4(aPos, 1.0f));
		}
		else
		{
			gl_ClipDistance[i] = 0;
		}
	}

	Normal = mat3(transpose(inverse(model))) * planes[planeIndex].xyz; 
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
in float TexCoord;

uniform sampler1D Texture;
uniform int lightIsOn;
uniform float minValue;
uniform float maxValue;
uniform int isEquivariance;//是否等差(1为等差,0为等比)
uniform float textureCoordRatio;//纹理坐标的系数

uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 normal, vec3 matColor, float AmbientOcclusion)
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
	vec3 diffuse = light.diffuse * (diff * matColor) * AmbientOcclusion;

	// specular
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir); 
	float spec = pow(max(dot(normal, halfwayDir), 0.0), light.shiness);
    vec3 specular = light.specular * (spec * 0.3) * AmbientOcclusion;  

	vec3 result = ambient + diffuse + specular;
	return result;
	
}

void main()
{
	vec3 normal = -Normal;
//	float distance = length(light.position - FragPos);
//    float attenuation = 1.0 / (distance * distance);
	vec3 color;
	float textCoord;
	if(isEquivariance == 0)
	{
		textCoord = log2(TexCoord/minValue)/log2(maxValue/minValue);
	}
	else
	{
		textCoord = (TexCoord-minValue)/(maxValue-minValue);
	}
	textCoord *= textureCoordRatio;
	color = vec3(texture(Texture, textCoord));	

    FragColor = vec4(calculateLightResult(normal,color,1.0), 1.0);
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}

#endif