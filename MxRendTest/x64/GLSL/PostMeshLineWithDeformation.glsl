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
	vec3 deformationpos = aPos + vec3(deformationScale.x*aDisplacement.x, deformationScale.y*aDisplacement.y, deformationScale.z*aDisplacement.z);
    gl_Position = projection * view * model * vec4(deformationpos, 1.0);
	
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
	FragColor = vec4(color, 1.0);
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
} 

#endif


