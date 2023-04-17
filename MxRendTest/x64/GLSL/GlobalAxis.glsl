#version 330 core

#ifdef vertex_shader
#include "extfunction.glsl"

layout (location = 0) in float aType;		//ID 坐标轴类型 0为X轴 1为Y轴 2为Z轴
layout (location = 1) in vec3 aVertex;		//顶点坐标（通用坐标系下）

out vec3 Color;
//通用视角下的PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;
//屏幕尺寸
uniform float uScr_width;
uniform float uScr_height;
//当前视景体尺寸
uniform float uPers_width;
uniform float uPers_height;

//当前视点状态下的X轴Y轴Z轴
uniform vec3 uAuxX_new;
uniform vec3 uAuxY_new;
uniform vec3 uAuxZ_new;


vec3 GetGlobleAxisPosition(int x, int y)
 {
		//旋转后为保证坐标轴显示在屏幕左下角需要移动的坐标。
		return vec3(-uAuxX_new * (0.5*uPers_width - x / uScr_width * uPers_width) - uAuxY_new * (0.5*uPers_height - y / uScr_height * uPers_height));		
 }
 
 void main()
{
	//传递颜色
	if(aType == 0)
	{
		Color = vec3(255,0,0);		
	}
	else if(aType == 1)
	{
		Color =  vec3(0,255,0);	
	}
	else if(aType == 2)
	{
		Color =  vec3(0,0,255);		
	}
	
	
	mat4 model;
	vec3 position = GetGlobleAxisPosition(75,75);//将坐标轴放在左下角 75X75像素处

	//首先判断X轴方向
	if(aType == 0)
	{ 	
		model = uModel_c * translate(position.x,position.y,position.z)*scale(0.8,0.8,0.8);
	}
	//Y轴
	else if(aType == 1)
	{
		vec3 axisRotate = vec3(0,0,1);
		model = uModel_c * translate(position.x,position.y,position.z)* rotate(axisRotate.x,axisRotate.y,axisRotate.z,90)*scale(0.8,0.8,0.8);
	}
	//Z轴
	else if(aType == 2)
	{
		vec3 axisRotate = vec3(0,1,0);
		model = uModel_c * translate(position.x,position.y,position.z)* rotate(axisRotate.x,axisRotate.y,axisRotate.z,-90)*scale(0.8,0.8,0.8);
	}

	gl_Position = uProjection_c * uView_c * model* vec4(aVertex, 1.0f);
	
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

in vec3 Color;
void main()
{	
	
	 FragColor = vec4(Color.x/255.0,Color.y/255.0,Color.z/255.0,1.0);
	 //FragColor = vec4(1.0,1.0,1.0,1.0);
	
} 
#endif