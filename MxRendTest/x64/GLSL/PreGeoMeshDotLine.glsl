#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = vec4(aPos, 1.0);
}
#endif

#ifdef geometry_shader
layout(lines) in;
layout(line_strip, max_vertices = 2) out;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out float texCoord; // 输出到片段着色器中计算线的模式
void main(void)
{
    vec3 pos0 = gl_in[0].gl_Position.xyz;
    vec3 pos1 = gl_in[1].gl_Position.xyz;
    float max_u_texture = length(pos1 - pos0);//使纹理坐标与线的长度成正比
    // 线段头
    gl_Position = projection * view * model * (gl_in[0].gl_Position);
    texCoord = 0.0;
    EmitVertex(); //发射顶点
    //线段尾
    gl_Position = projection * view * model * (gl_in[1].gl_Position);
    texCoord = max_u_texture;
    EmitVertex();//发射顶点
    EndPrimitive();//顶点发送完毕
}


#endif

#ifdef fragment_shader

uniform vec3 showColor;
out vec4 FragColor;
uniform int pattern = 0x3F3F;// 一个介于0和0xFFFF之间的整数，代表画线的比特模式。
uniform int nPatterns = 200; // 图案的数量/视口的单位长度，通常为200-300，以保证良好的图案密度。
in float texCoord;


void main()
{	
    //使用4个字节的掩码模式
   //将纹理坐标映射到区间[0,2*8]
    uint bitpos = uint(round(texCoord * nPatterns)) % 16U;
    // 移动一个单位1U到bitpos位置，以便位是1到1000之间的整数 0000 //0000 0000 = 0x8000
    uint bit = (1U << bitpos);
    // 测试掩码
    //  Line::SOLID:       pattern = 0xFFFF;  // = 1111 1111 1111 1111 
    //  Line::DASH:        pattern = 0x3F3F;  // = 0011 1111 0011 1111
    //  Line::DOT:         pattern = 0x6666;  // = 0110 0110 0110 0110
    //  Line::DASHDOT:     pattern = 0xFF18;  // = 1111 1111 0001 1000
    //  Line::DASHDOTDOT:  pattern = 0x7E66;  // = 0111 1110 0110 0110
    uint up = uint(pattern);
    // 如果该位与掩码模式不匹配，则丢弃该位
    if ((up & bit) == 0U) discard;
    FragColor = vec4(showColor,1);
} 

#endif


