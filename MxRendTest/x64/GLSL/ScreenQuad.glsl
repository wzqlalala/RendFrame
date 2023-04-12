#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec2 aPos;

uniform int width;
uniform int height;


void main()
{
    gl_Position = vec4(2.0*aPos.x/width-1, 1-2.0*aPos.y/height,-0.99,1.0);
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

uniform vec4 showColor;
void main()
{
    FragColor = showColor;
} 
#endif