#version 450 core

#ifdef vertex_shader
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aMaterial;
layout(location = 2) in float aIsColor;
layout(location = 3) in float aValue;
layout(location = 4) in vec3 aDisplacement;

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
out vec3 Normal;

//光照部分
out vec3 FragPos;

void main()
{
	vec3 deformationPos = aPos + deformationScale * aDisplacement;
	FragPos = vec3(model * vec4(deformationPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);

	Value  = aValue;
	isColor = int(aIsColor) / 2;
	material = aMaterial;
	hasValue = int(aIsColor) % 2;
	gl_PointSize = PointSize;

	for (int i = 0; i < 8; ++i) {
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
in vec3 Normal;

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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform Light light;

vec3 calculateLightResult(vec3 norm, vec3 color, float AmbientOcclusion, float z)
{
    if(lightIsOn == 0) return color;

    vec3 viewDir = normalize(viewPos - FragPos);
    // if (dot(norm, viewDir) < 0.0) norm = -norm;
    norm = normalize(viewDir);

    vec3 ambient = light.ambient * color;//ambi
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color * AmbientOcclusion * z;//diffu
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), light.shiness);
    vec3 specular = light.specular * spec * 0.3 * AmbientOcclusion * z;//specu
    return ambient + diffuse + specular;
}

void main()
{
    vec3 N;
    N.xy = gl_PointCoord * 2.0f - 1.0f;
    
    float mag = dot(N.xy,N.xy);
    if(mag > 0.98f) discard;
    N.z = sqrt(1.0f - mag);
    // N = normalize(N);
    vec3 color = isColor == 0 ? material : hasValue == 1 ? vec3(texture(Texture, (isEquivariance == 0 ? log2(Value/minValue) / log2(maxValue/minValue) : (Value-minValue) / (maxValue-minValue)) * textureCoordRatio)) : vec3(0.5f);

	FragColor.rgb = pow(calculateLightResult(Normal, color, 1.0, N.z), vec3(1.0 / 2.2));
    FragColor.a = 1.0;
} 

#endif