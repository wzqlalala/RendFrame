#version 330 core
#ifdef vertex_shader
#include "extfunction.glsl"
layout (location = 0) in float aType;				//0为X 1为Y 2为Z
layout (location = 1) in vec2 aVertex;				//纹理顶点坐标（像素坐标系下 每个纹理都在0,0点）
layout (location = 2) in vec2 aTexCoord;			//文字纹理坐标

out vec2 TexCoord;		//纹理坐标
out vec3 Color;			//颜色

//文字视角下的PVM
uniform mat4 uView_font;
uniform mat4 uProjection_font;
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

//传入自适应分辨率比例
uniform float uRatio;


//获取全局坐标轴在屏幕左下角显示的位置 x y 为屏幕左下角为原点的像素坐标
vec3 GetGlobleAxisPosition(int x, int y)
{
	//旋转后为保证坐标轴显示在屏幕左下角需要移动的坐标。
	return vec3(-uAuxX_new * (0.5*uPers_width - x / uScr_width * uPers_width) - uAuxY_new * (0.5*uPers_height - y / uScr_height * uPers_height));		
}
void main()
{
	Color = vec3(1,1,1);//白色
	TexCoord = vec2(aTexCoord.x/1024.0,aTexCoord.y/1024.0);  //1024为纹理的像素大小

	mat4 model;
	vec3 position = GetGlobleAxisPosition(80,80);//将坐标轴放在左下角 80X80像素处

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

	//获取文字经过变换后在通用视角下的坐标
	vec3 FontPos_c_beforeTrans = vec3(1.0,0,0);	//文字变换前在同通用坐标系下的坐标
	vec3 FontPos_c_afterTrans =  (model* vec4(FontPos_c_beforeTrans, 1.0f)).xyz; //文字变换后在通用坐标系下的坐标
	//获取文字经过变换后在标准坐标系下坐标	
	vec3 normPos = (uProjection_c *uView_c * uModel_c*vec4(FontPos_c_afterTrans,1.0)).xyz;
	//文字添加点在像素坐标系下坐标（原点在左上角）
	vec2 pixelPos;
	pixelPos.x = (normPos.x * uScr_width / 2 + uScr_width / 2);
	pixelPos.y = (-normPos.y * uScr_height / 2 + uScr_height / 2);

	mat4 model_2;
	model_2 = translate(pixelPos.x,uScr_height-pixelPos.y,0)*scale(0.3*uRatio,0.3*uRatio,0.3*uRatio);  //调整文字大小
	gl_Position = uProjection_font * uView_font * model_2 * vec4(aVertex,0,1);

    

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
	FragColor = vec4(Color,1.0)* sampled;
     
}
#endif




