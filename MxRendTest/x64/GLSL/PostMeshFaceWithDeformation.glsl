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

out float Value;
flat out int isColor;
out vec3 material;
flat out int hasValue;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	vec3 deformationPos = aPos + deformationScale * aDisplacement;
	FragPos = vec3(model * vec4(deformationPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	
	Value  = aValue;
	isColor = int(aIsColor);
	material = aMaterial;
	hasValue = int(aHasValue);

	for(int i = 0;i < 8; ++i){
		gl_ClipDistance[i] = dot(planes[i], vec4(deformationPos, 1.0f));	
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
in float Value;
in vec3 material;
flat in int isColor;
flat in int hasValue;

uniform sampler1D Texture;
uniform int lightIsOn;
uniform float minValue;
uniform float maxValue;
uniform int isEquivariance;
uniform float textureCoordRatio;
uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 normal, vec3 matColor)
{
	if(lightIsOn == 0)
	{
		return matColor;
	}
	vec3 ambient = light.ambient * matColor;	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * matColor);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir); 
	float spec = pow(max(dot(normal, halfwayDir), 0.0), light.shiness);
    vec3 specular = light.specular * (spec * 0.3);  

	return ambient + diffuse + specular;
}

void main()
{
	vec3 normal = gl_FrontFacing ? Normal : -Normal;
	vec3 color = isColor == 0 ? material : hasValue == 1 ? vec3(texture(Texture, (isEquivariance == 0 ? log2(Value/minValue) / log2(maxValue/minValue) : (Value-minValue) / (maxValue-minValue)) * textureCoordRatio)) : vec3(0.5f);
	FragColor = vec4(calculateLightResult(normal, color), 1.0);
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));
} 
#endif
