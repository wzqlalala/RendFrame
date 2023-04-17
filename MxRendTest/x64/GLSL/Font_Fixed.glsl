#version 330 core
#ifdef vertex_shader
#include "extfunction.glsl"
layout (location = 0) in vec3  aColor;				//颜色
layout (location = 1) in float aSize;				//大小
layout (location = 2) in vec2 aVertex;				//纹理顶点坐标（像素坐标系下 每个纹理都在0,0点）
layout (location = 3) in vec2 aTexCoord;			//文字纹理坐标
layout (location = 4) in vec2 aPointPos;			//位置坐标（在 x（-1,1） y（-1,1）之间） 屏幕中心为原点

out vec2 TexCoord;		//纹理坐标
out vec3 Color;			//颜色

//传入自适应分辨率比例
uniform float uRatio;
//像素坐变换矩阵
uniform mat4 uProjection_font;
uniform mat4 uView_font;
uniform mat4 uModel_font;

//屏幕尺寸
uniform float uScr_width;
uniform float uScr_height;

void main()
{
	Color = vec3(aColor.x/255.0,aColor.y/255.0,aColor.z/255.0);
	TexCoord = vec2(aTexCoord.x/1024.0,aTexCoord.y/1024.0);  //1024为纹理的像素大小
	mat4 Model;
	Model = translate(uScr_width/2+aPointPos.x*uScr_width/2,uScr_height/2+aPointPos.y*uScr_height/2,0)*scale(0.3*uRatio*aSize,0.3*uRatio*aSize,0.3*uRatio*aSize);  //调整文字大小  调整到以屏幕中心为原点
	gl_Position = uProjection_font * uView_font *Model* vec4(aVertex,0,1);
}
#endif


#ifdef fragment_shader
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Color;
uniform sampler2D Texture;
void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Texture, TexCoord).r);
	FragColor = vec4( Color,1.0)* sampled;
}   
#endif