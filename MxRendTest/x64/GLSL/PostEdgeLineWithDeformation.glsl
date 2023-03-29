#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aDisplacement;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 deformationScale;


uniform vec4 planes[8];
float gl_ClipDistance[8];

out vec3 deformationpos;

out VS_OUT 
{
	vec3 deformationpos;
} vs_out;

void main()
{
	vs_out.deformationpos = aPos + vec3(deformationScale.x*aDisplacement.x, deformationScale.y*aDisplacement.y, deformationScale.z * aDisplacement.z);
    gl_Position = projection * view * model * vec4(vs_out.deformationpos, 1.0f);

	for(int i = 0;i < 8; ++i)
	{
		gl_ClipDistance[i] = dot(planes[i],vec4(vs_out.deformationpos, 1.0f));	
	}

}
#endif

#ifdef geometry_shader
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT
{
	vec3 deformationpos;
} gs_in[];

uniform vec4 showColor;
uniform float rightToLeft;
uniform float lineWidth;

out vec4 fColor;

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
	float meshSize = distance(gs_in[0].deformationpos,gs_in[1].deformationpos);
	float ratio = rightToLeft/meshSize;
	if(ratio < 4000)
	{
		fColor = vec4(showColor.xyz, 1 - sqrt(ratio / 4000));
	}
	else
	{
		fColor = vec4(showColor.xyz, 0.1);
	}
	vec2 dir = normalize(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy) * (lineWidth/2.0)/500.0;
    vec2 normal = vec2(-dir.y, dir.x);
	

	gl_Position = gl_in[0].gl_Position + vec4(normal, 0.0, 0.0)/* - vec4(dir, 0.0, 0.0)*/;
	outclipdistance(0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(normal, 0.0, 0.0)/* + vec4(dir, 0.0, 0.0)*/;
	outclipdistance(1);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position - vec4(normal, 0.0, 0.0)/* - vec4(dir, 0.0, 0.0)*/;
	outclipdistance(0);
    EmitVertex();

    gl_Position = gl_in[1].gl_Position - vec4(normal, 0.0, 0.0)/* + vec4(dir, 0.0, 0.0)*/;
	outclipdistance(1);
    EmitVertex();

    EndPrimitive();

}

#endif

#ifdef fragment_shader
out vec4 FragColor;

in vec4 fColor;

void main()
{	
//	gl_FragDepth+=0.0000000001f;
	FragColor = fColor;
} 
#endif