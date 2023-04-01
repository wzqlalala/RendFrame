#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
#endif

#ifdef fragment_shader
out vec4 FragColor;
uniform vec4 showColor;
void main()
{	
	 FragColor = vec4(0.5);
} 
#endif