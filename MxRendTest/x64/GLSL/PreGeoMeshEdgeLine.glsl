#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT 
{
	vec3 pos;
} vs_out;

void main()
{
    vs_out.pos = aPos;
	gl_Position = projection * view * model * vec4(vs_out.pos, 1.0f);
    // gl_Position.z = gl_Position.z - 0.00001;
}
#endif

#ifdef geometry_shader
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT
{
	vec3 pos;
} gs_in[];

uniform vec4 showColor;
uniform float rightToLeft;
uniform float lineWidth;

out vec4 fColor;

void main() 
{    
	float ratio = rightToLeft / distance(gs_in[0].pos, gs_in[1].pos);
    fColor = (ratio < 4000) ? vec4(showColor.xyz, 1 - sqrt(ratio / 4000)) : vec4(showColor.xyz, 0.1);
	vec2 dir = normalize(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy) * (lineWidth/2.0)/500.0;
    vec2 normal = vec2(-dir.y, dir.x);
	
	gl_Position = gl_in[0].gl_Position + vec4(normal, 0.0, 0.0)/* - vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(normal, 0.0, 0.0)/* + vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position - vec4(normal, 0.0, 0.0)/* - vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position - vec4(normal, 0.0, 0.0)/* + vec4(dir, 0.0, 0.0)*/;
    EmitVertex();

    EndPrimitive();

}

#endif

#ifdef fragment_shader
out vec4 FragColor;

in vec4 fColor;

void main()
{	
	FragColor = fColor;
} 
#endif