#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 outColor;

void main()
{
    gl_Position = vec4(aPos, 0.99f, 1.0f);
	outColor = aColor;
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

in vec3 outColor;

void main()
{
    FragColor = vec4(outColor,1.0);
} 
#endif