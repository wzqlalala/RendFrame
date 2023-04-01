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

uniform vec4 planes[8];
//float gl_ClipDistance[8];

out float Value;
flat out int isColor;
out vec3 material;
flat out int hasValue;

void main()
{
	vec3 deformationPos = aPos + deformationScale * aDisplacement;
	gl_Position = projection * view * model * vec4(deformationPos, 1.0);
	
	Value  = aValue;
	isColor = int(aIsColor);
	material = aMaterial;
	hasValue = int(aHasValue);

	for(int i = 0;i < 8; ++i){
		gl_ClipDistance[i] = dot(planes[i],vec4(deformationPos, 1.0f));	
	}
}
#endif



#ifdef fragment_shader


out vec4 FragColor;

in float Value;
in vec3 material;
flat in int isColor;
flat in int hasValue;

uniform sampler1D Texture;
uniform float minValue;
uniform float maxValue;
uniform int isEquivariance;//是否等差(1为等差,0为等比)
uniform float textureCoordRatio;//纹理坐标的系数

void main()
{
//	float distance = length(light.position - FragPos);
//    float attenuation = 1.0 / (distance * distance);
	vec3 color = isColor == 0 ? material : hasValue == 1 ? vec3(texture(Texture, (isEquivariance == 0 ? log2(Value/minValue) / log2(maxValue/minValue) : (Value-minValue) / (maxValue-minValue)) * textureCoordRatio)) : vec3(0.5f);
    FragColor.rgb = pow(color, vec3(1.0/2.2));
} 

#endif


