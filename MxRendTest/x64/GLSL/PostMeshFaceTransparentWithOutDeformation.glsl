#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

out VS_OUT 
{
	vec3 pos;
} vs_out;

void main()
{
	vs_out.pos = aPos;
}
#endif

#ifdef geometry_shader
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 pos;
} gs_in[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 planes[8];

out vec3 Normal;
out vec3 FragPos;

void main() 
{
	Normal = normalize(mat3(transpose(inverse(model))) * cross(gs_in[1].pos -  gs_in[0].pos, gs_in[2].pos -  gs_in[1].pos));
    for (int i = 0; i < 3; ++i) {
		FragPos = vec3(model * vec4(gs_in[i].pos, 1.0));
		gl_Position = projection * view * vec4(FragPos, 1.0);	
		for(int j = 0;j < 8; ++j)
		{
			gl_ClipDistance[j] = dot(planes[j], vec4(gs_in[i].pos, 1.0f));	
		}
        EmitVertex();
    }
    EndPrimitive();
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
	vec3 color = vec3(1,1,1);
	FragColor = vec4(calculateLightResult(normal,color), 0.3);
} 

#endif


