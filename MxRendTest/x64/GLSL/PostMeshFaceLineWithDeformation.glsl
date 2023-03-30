#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aDisplacement;
layout (location = 2) in float aIsTriangle;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 deformationScale;
uniform vec4 planes[8];

out VS_OUT {
    int isTriangle;
    vec3 deformationPos;
} vs_out;

void main() {
    vs_out.deformationPos = aPos + deformationScale * aDisplacement;
    gl_Position = projection * view * model * vec4(vs_out.deformationPos, 1.0f);

    vs_out.isTriangle = int(aIsTriangle);

    for (int i = 0; i < 8; ++i) {
        gl_ClipDistance[i] = dot(planes[i], vec4(vs_out.deformationPos, 1.0f));
    }
}
#endif

#ifdef geometry_shader
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

in VS_OUT {
    int isTriangle;
    vec3 deformationPos;
} gs_in[];

uniform vec4 showColor;
uniform float rightToLeft;

out vec4 fColor;

void outputClipDistances(int index) {
    for (int i = 0; i < 8; ++i) {
        gl_ClipDistance[i] = gl_in[index].gl_ClipDistance[i];
    }
}

void main() {
    float ratio = rightToLeft / distance(gs_in[0].deformationPos, gs_in[1].deformationPos);
    fColor = (ratio < 800) ? vec4(showColor.xyz, 1 - sqrt(ratio / 800)) : vec4(showColor.xyz, 0.0);

    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
        outputClipDistances(i);
        EmitVertex();
    }
    if (gs_in[0].isTriangle == 1) {
        gl_Position = gl_in[0].gl_Position;
        outputClipDistances(0);
        EmitVertex();
    } 
    EndPrimitive();
}
#endif

#ifdef fragment_shader
in vec4 fColor;

out vec4 FragColor;

void main() {
    FragColor = fColor;
}
#endif