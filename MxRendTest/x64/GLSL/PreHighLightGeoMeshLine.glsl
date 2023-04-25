#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position.z = gl_Position.z - 0.0001;
}
#endif



#ifdef fragment_shader

uniform vec3 showColor;
out vec4 FragColor;

void main()
{	
    FragColor = vec4(showColor,1);
} 

#endif


