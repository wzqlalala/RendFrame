#version 430 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aMaterial;

out VS_OUT {
    vec3 FragPos;
	vec3 Normal;
	vec3 Material;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  
    vs_out.Material = aMaterial;
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
	vec3 Normal;
	vec3 Material;
} fs_in;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Light light;

void main()
{           
	vec3 normal;
	if( gl_FrontFacing ) 
	{
		normal = fs_in.Normal;
	}
	else
	{
		normal = -fs_in.Normal;
	}

    // ambient
    vec3 ambient = light.ambient * fs_in.Material;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * fs_in.Material);
    
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64);
    vec3 specular = light.specular * (spec * 0.5);  
        
    vec3 result = ambient + diffuse + specular;
    //vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
#endif