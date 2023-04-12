#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aIsTriangle;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    int isTriangle;
    vec3 deformationPos;
} vs_out;

void main() {
    vs_out.deformationPos = aPos;
    gl_Position = projection * view * model * vec4(vs_out.deformationPos, 1.0f);

    vs_out.isTriangle = int(aIsTriangle);
}
#endif

#ifdef geometry_shader
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT {
    int isTriangle;
    vec3 deformationPos;
} gs_in[];

void main() {
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    if (gs_in[0].isTriangle == 1) {
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
    } 
    EndPrimitive();
}
#endif

#ifdef fragment_shader

out vec4 FragColor;

void main() {
    FragColor = vec4(1,1,1,1);
}
#endif