#version 330 core
#ifdef vertex_shader
	
#include "extfunction.glsl"
layout (location = 0) in vec3 aPos;		//顶点坐标（通用坐标系下）
layout (location = 1) in vec3 aPointPos;//位置坐标（模型坐标系下）
layout (location = 2) in vec3 aDirection; //方向
layout (location = 3) in float aSize; //大小
layout (location = 4) in vec3 aColor;	//颜色RGB


out vec3 Color;
out float Depth;
//通用视角下的PVM
uniform mat4 uModel_c;
uniform mat4 uView_c;
uniform mat4 uProjection_c;

//模型视角下的PVM
uniform mat4 uModel_m;
uniform mat4 uView_m;
uniform mat4 uProjection_m;


void main()
{
	//传递颜色
	Color = aColor;
	
	mat4 model;
	//将载荷添加点从模型坐标系转化到通用坐标系下坐标
	vec3 pos_norm = ModelToNormPositon(aPointPos,uProjection_m,uView_m,uModel_m);
	Depth = 0.5+pos_norm.z*0.5-0.03;  //输出深度值 让箭头的深度值和添加点的深度值保持一致
	vec3 pos_c = NormToModelPositon(pos_norm,uProjection_c,uView_c,uModel_c);

	//根据值设定箭头的方向
	float angle = degrees(acos(dot(vec3(1,0,0),aDirection)/length(aDirection)));//获取旋转角度
	if(abs(angle)<0.1) //判断夹角为0
	{
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*scale(0.3*aSize,0.3*aSize,0.3*aSize);
	}	
	else if((abs(angle)-180<0.1) && (abs(angle)-180>-0.1))//判断夹角为180
	{
		vec3 axisRotate = vec3(0,0,1);				//获取旋转轴
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle)*scale(0.2*aSize,0.2*aSize,0.2*aSize);
	}
	else
	{
		vec3 axisRotate = normalize(cross(vec3(1,0,0),aDirection));				//获取旋转轴
		model = uModel_c*translate(pos_c.x,pos_c.y,pos_c.z)*rotate(axisRotate.x,axisRotate.y,axisRotate.z,angle)*scale(0.2*aSize,0.2*aSize,0.2*aSize);
	}

	gl_Position = uProjection_c * uView_c * model* vec4(aPos, 1.0f);
	
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

in vec3 Color;
in float Depth;


void main()
{	
	FragColor = vec4(Color, 1.0);	 
	//手动修改深度值，使其与添加载荷边界点的深度值保持一致
	gl_FragDepth = Depth;
}
#endif