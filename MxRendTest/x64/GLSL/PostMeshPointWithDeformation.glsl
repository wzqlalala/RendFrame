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

out float Value;
flat out int isColor;
out vec3 material;
flat out int hasValue;

//光照部分
out vec3 FragPos;

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
uniform int lightIsOn;
uniform float minValue;
uniform float maxValue;
uniform int isEquivariance;//是否等差(1为等差,0为等比)
uniform float textureCoordRatio;//纹理坐标的系数

uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 norm, vec3 color, float AmbientOcclusion)
{
	if(lightIsOn == 0) return color;

	vec3 viewDir = normalize(viewPos - FragPos);
	if (dot(norm, viewDir) < 0.0) norm = -norm;
	norm = normalize(norm);

	vec3 ambient = light.ambient * color;//ambi
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * color * AmbientOcclusion;//diffu
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), light.shiness);
	vec3 specular = light.specular * spec * 0.3 * AmbientOcclusion;//specu
	return ambient + diffuse + specular;
	
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

	float textCoord = isEquivariance == 1 ? (Value - minValue) / (maxValue - minValue) : log2(Value / minValue) / log2(maxValue / minValue);
	textCoord *= textureCoordRatio;
	vec3 color = texture(Texture, textCoord).rgb;

	FragColor.rgb = pow(calculateLightResult(normal, color, 1.0), vec3(1.0 / 2.2));
	FragColor.a = 1.0;
} 

#endif


