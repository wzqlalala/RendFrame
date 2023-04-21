#version 450 core

#ifdef vertex_shader
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aMaterial;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int PointSize;

out vec3 material;

//光照部分
out vec3 FragPos;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	material = aMaterial;
	gl_PointSize = PointSize;
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
in vec3 material;

uniform sampler2D sprite_texture;
uniform int lightIsOn;

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
    vec4 color = texture(sprite_texture, gl_PointCoord);
	// if(color == vec4(0,0,0,1))
	// {
	// 	discard;
	// }
    if(all(lessThan(color.rgb, vec3(0.1))))
    {
        discard;
    }
    // FragColor.rgb = material;
    // FragColor.a = 1.0f;
    // FragColor = color;
    vec3 N;
    N.xy = gl_PointCoord * 2.0f - 1.0f; 
    float mag = dot(N.xy,N.xy);
    N.z = sqrt(1.0f - mag);
	FragColor.rgb = pow(calculateLightResult(N, material, 1.0, N.z), vec3(1.0 / 2.2));
    // FragColor.rgb = material;
    FragColor.a = 1.0f;
} 

#endif