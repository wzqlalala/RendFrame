#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aPlaneIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 planes[8];

out float TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main() {
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = aTexCoord;
	int planeIndex = int(aPlaneIndex);

	for (int i = 0; i < 8; ++i)
		gl_ClipDistance[i] = (planeIndex != i) ? dot(planes[i], vec4(aPos, 1.0f)) : 0;
	Normal = mat3(transpose(inverse(model))) * planes[planeIndex].xyz;
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

struct Light {
	vec3 position, ambient, diffuse, specular;
	float shiness;
};

in vec3 FragPos, Normal;
in float TexCoord;

uniform sampler1D Texture;
uniform int lightIsOn, isEquivariance;
uniform float minValue, maxValue, textureCoordRatio;
uniform vec3 viewPos;
uniform Light light;

vec3 calcLight(vec3 norm, vec3 color, float AmbientOcclusion) {
	if (lightIsOn == 0) return color;

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

void main() {
	float textCoord = isEquivariance == 1 ? (TexCoord - minValue) / (maxValue - minValue) : log2(TexCoord / minValue) / log2(maxValue / minValue);
	textCoord *= textureCoordRatio;
	vec3 color = texture(Texture, textCoord).rgb;

	FragColor.rgb = pow(calcLight(Normal, color, 1.0), vec3(1.0 / 2.2));
	FragColor.a = 1.0;
}
#endif