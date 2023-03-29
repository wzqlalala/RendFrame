#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aMaterial;
layout (location = 2) in float aIsColor;
layout (location = 3) in float aValue;
layout (location = 4) in vec3 aDisplacement;
layout (location = 5) in float aHasValue;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 deformationScale;
uniform int PointSize;

uniform vec4 planes[8];
float gl_ClipDistance[8];

out float Value;
flat out int isColor;
out vec3 material;
flat out int hasValue;

//光照部分
out vec3 FragPos;

void main()
{
	vec3 deformationpos = aPos + vec3(deformationScale.x*aDisplacement.x, deformationScale.y*aDisplacement.y, deformationScale.z*aDisplacement.z);
	FragPos = vec3(model * vec4(deformationpos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
	gl_PointSize = PointSize;
	
	Value  = aValue;
	isColor = int(aIsColor);
	material = aMaterial;
	hasValue = int(aHasValue);

	for(int i = 0;i < 8; ++i)
	{
		gl_ClipDistance[i] = dot(planes[i],vec4(deformationpos, 1.0f));	
	}
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

in float Value;
in vec3 material;
flat in int isColor;
flat in int hasValue;

uniform sampler1D Texture;
uniform int drawNum;
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
	//圆形
	float r = distance(gl_PointCoord, vec2(0.5, 0.5));
	//根据距离设置片元
	if(r >= 0.5)
	{
		discard;
	}
	vec3 normal = vec3(0.0, 0.0, 1.0);
	if(dot(normal, viewPos) < 0) 
	{
		normal = -normal;
	}

	//	float distance = length(light.position - FragPos);
	//  float attenuation = 1.0 / (distance * distance);
	vec3 color;
	if(isColor == 0)//部件材质颜色与光照
	{
		color = material;
	}
	else if(isColor == 1)
	{
		if(hasValue == 1)//纹理与光照
		{
			float textCoord;
			if(isEquivariance == 0)
			{
				textCoord = log2(Value/minValue)/log2(maxValue/minValue);
			}
			else
			{
				textCoord = (Value-minValue)/(maxValue-minValue);
			}
			textCoord *= textureCoordRatio;
			color = vec3(texture(Texture, textCoord));			
		}
		else
		{
			color = vec3(0.5,0.5,0.5);			
		}
	}
	FragColor = vec4(calculateLightResult(normal,color,1.0), 1.0);
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
} 

#endif


