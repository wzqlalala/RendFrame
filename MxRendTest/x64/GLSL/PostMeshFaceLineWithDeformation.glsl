#version 330 core


#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aDisplacement;
layout (location = 2) in float aIsTriangle;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 deformationScale;

uniform vec4 planes[8];

out VS_OUT 
{
    int istriangle;
	vec3 deformationpos;
} vs_out;

void main()
{
	vs_out.deformationpos = aPos + vec3(deformationScale.x*aDisplacement.x, deformationScale.y*aDisplacement.y, deformationScale.z * aDisplacement.z);
    gl_Position = projection * view * model * vec4(vs_out.deformationpos, 1.0f);
	
	vs_out.istriangle = int(aIsTriangle);
		
	for(int i = 0;i < 8; ++i)
	{
		gl_ClipDistance[i] = dot(planes[i],vec4(vs_out.deformationpos, 1.0f));	
	}

}
#endif

#ifdef geometry_shader
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT
{
	int istriangle;
	vec3 deformationpos;
} gs_in[];

uniform vec4 showColor;
uniform float rightToLeft;

out vec4 fColor;
//out float fmeshSize;

void outclipdistance(int index)
{
	gl_ClipDistance[0] = gl_in[index].gl_ClipDistance[0];
	gl_ClipDistance[1] = gl_in[index].gl_ClipDistance[1];
	gl_ClipDistance[2] = gl_in[index].gl_ClipDistance[2];
	gl_ClipDistance[3] = gl_in[index].gl_ClipDistance[3];
	gl_ClipDistance[4] = gl_in[index].gl_ClipDistance[4];
	gl_ClipDistance[5] = gl_in[index].gl_ClipDistance[5];
	gl_ClipDistance[6] = gl_in[index].gl_ClipDistance[6];
	gl_ClipDistance[7] = gl_in[index].gl_ClipDistance[7];
}

void main() 
{    
//	float meshSize = distance( gl_in[0].gl_Position,gl_in[1].gl_Position);
//	if(meshSize > 0.005)
//	{
//		fColor = vec4(showColor.xyz, 0.00004* meshSize - 0.0002);
//	}
//	fmeshSize = meshSize;
	float meshSize = distance( gs_in[0].deformationpos,gs_in[1].deformationpos);
	float ratio = rightToLeft/meshSize;
	if(ratio < 800)
	{
		fColor = vec4(showColor.xyz, 1 - sqrt(ratio / 800));
	}
	else
	{
		fColor = vec4(showColor.xyz, 0.0);
	}
	
	if(gs_in[0].istriangle == 1)
	{
		gl_Position =  gl_in[0].gl_Position;
		outclipdistance(0);
		EmitVertex();
		gl_Position =  gl_in[1].gl_Position;
		outclipdistance(1);
		EmitVertex();
		gl_Position =  gl_in[2].gl_Position;
		outclipdistance(2);
		EmitVertex();		
		gl_Position =  gl_in[0].gl_Position;
		outclipdistance(0);
		EmitVertex();		
		EndPrimitive();
	}
	else
	{
		gl_Position =  gl_in[0].gl_Position;
		outclipdistance(0);
		EmitVertex();
		gl_Position =  gl_in[1].gl_Position;
		outclipdistance(1);
		EmitVertex();
		gl_Position =  gl_in[2].gl_Position;
		outclipdistance(2);
		EmitVertex();		
		EndPrimitive();
		
	}
		 
//	showColor = gs_in[0].showColor;
}



#endif

#ifdef fragment_shader
in vec4 fColor;

out vec4 FragColor;

void main()
{	
	 FragColor = fColor;
} 
#endif

//#ifdef vertex_shader
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aDisplacement;
//layout (location = 2) in float aIsTriangle;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform vec3 deformationScale;
//
////uniform float lineWidth;
//
//uniform vec4 plane0;
//uniform vec4 plane1;
//uniform vec4 plane2;
//uniform vec4 plane3;
//uniform vec4 plane4;
//uniform vec4 plane5;
//uniform vec4 plane6;
//uniform vec4 plane7;
//
////out VS_OUT 
////{
////    int istriangle;
////	vec3 deformationpos;
////} vs_out;
//vec3 deformationpos;
//void main()
//{
//	deformationpos = aPos + vec3(deformationScale.x*aDisplacement.x, deformationScale.y*aDisplacement.y, deformationScale.z * aDisplacement.z);
//    gl_Position = projection * view * model * vec4(deformationpos, 1.0f);
//		
//	gl_ClipDistance[0] = dot(plane0,vec4(deformationpos, 1.0f));
//	gl_ClipDistance[1] = dot(plane1,vec4(deformationpos, 1.0f));
//	gl_ClipDistance[2] = dot(plane2,vec4(deformationpos, 1.0f));
//	gl_ClipDistance[3] = dot(plane3,vec4(deformationpos, 1.0f));
//	gl_ClipDistance[4] = dot(plane4,vec4(deformationpos, 1.0f));
//	gl_ClipDistance[5] = dot(plane5,vec4(deformationpos, 1.0f));
//	gl_ClipDistance[6] = dot(plane6,vec4(deformationpos, 1.0f));
//	gl_ClipDistance[7] = dot(plane7,vec4(deformationpos, 1.0f));
//
//}
//#endif
//
//#ifdef fragment_shader
//uniform vec4 showColor;
//
//out vec4 FragColor;
//
//void main()
//{	
//	 FragColor = showColor;
//} 
//#endif