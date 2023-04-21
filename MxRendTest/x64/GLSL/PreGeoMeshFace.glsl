#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aMaterial;

out VS_OUT 
{
	vec3 pos;
	vec3 material;
} vs_out;

void main()
{
	vs_out.pos = aPos;
	vs_out.material = aMaterial;
}
#endif

#ifdef geometry_shader
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 pos;
	vec3 material;
} gs_in[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec3 material;

void main() 
{
	Normal = normalize(mat3(transpose(inverse(model))) * cross(gs_in[1].pos -  gs_in[0].pos, gs_in[2].pos -  gs_in[1].pos));
	material = gs_in[0].material;
    for (int i = 0; i < 3; ++i) {
		FragPos = vec3(model * vec4(gs_in[i].pos, 1.0));
		gl_Position = projection * view * vec4(FragPos, 1.0);	
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
in vec3 material;

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
	FragColor = vec4(calculateLightResult(normal,material), 1.0);
} 

#endif


