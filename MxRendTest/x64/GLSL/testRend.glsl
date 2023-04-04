#version 450 core

#ifdef vertex_shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
#endif

#ifdef fragment_shader
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
//	FragColor = vec4(1.0,0.0,0.0,1.0);
}
#endif


//#version 450 core
//
//#ifdef vertex_shader
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aMaterial;
//layout (location = 2) in float aIsColor;
//layout (location = 3) in float aValue;
//layout (location = 4) in vec3 aDisplacement;
//layout (location = 5) in float aHasValue;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform vec3 deformationScale;
//uniform int PointSize;
//
//uniform vec4 planes[8];
//
//out VS_OUT {
//    float Value;
//    flat int isColor;
//    vec3 material;
//    flat int hasValue;
//    vec3 FragPos;
//    vec3 Pos;
//    int PointSize;
//} vs_out;
//
//void main()
//{
//    vec3 deformationPos = aPos + deformationScale * aDisplacement;
//    vec3 FragPos = vec3(model * vec4(deformationPos, 1.0));
//
//    vs_out.Value = aValue;
//    vs_out.isColor = int(aIsColor);
//    vs_out.material = aMaterial;
//    vs_out.hasValue = int(aHasValue);
//    vs_out.Pos = deformationPos;
//    vs_out.PointSize = PointSize;
//    vs_out.FragPos = FragPos;
//
//    gl_Position = projection * view * vec4(FragPos, 1.0);
//    for(int i = 0; i < 8; ++i){
//        gl_ClipDistance[i] = dot(planes[i], vec4(deformationPos, 1.0f));
//    }
//}
//#endif
//
//#ifdef geometry_shader
//layout(points) in;
//layout(triangle_strip, max_vertices = 64) out;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//in VS_OUT {
//    float Value;
//    flat int isColor;
//    vec3 material;
//    flat int hasValue;
//    vec3 FragPos;
//    vec3 Pos;
//    int PointSize;
//} gs_in[];
//
//out vec3 Normal;
//out vec3 FragPos;
//flat out float Value;
//flat out int isColor;
//flat out int hasValue;
//out vec3 material;
//
//const float pi = 3.14159;
//
//void main() {
//    Value = gs_in[0].Value;
//    isColor = gs_in[0].isColor;
//    hasValue = gs_in[0].hasValue;
//    material = gs_in[0].material;
//
//    // Generate sphere
//    // for(int j = 0; j <= 10; j++) {
//    //     float y = sin(float(j) * pi / 10.0);
//    //     float r = cos(float(j) * pi / 10.0);
//
//    //     for(int i = 0; i <= 5; i++) {
//    //         float x = r * sin(float(i) * pi * 2.0 / 5.0);
//    //         float z = r * cos(float(i) * pi * 2.0 / 5.0);
//
//    //         // Calculate position
//    //         vec3 position = gs_in[0].Pos + gs_in[0].PointSize * vec3(x, y, z);
//
//    //         // Output vertex
//    //         gl_Position = projection * view * vec4(position, 1.0);
//    //         EmitVertex();
//    //     }
//    //     EndPrimitive();
//    // }
//
//    // ��������
//    for (float u = 0.0; u < 2.0 * 3.141592; u += 3.141592 / 4.0) 
//    {
//        for (float v = -3.141592 / 2.0; v < 3.141592 / 2.0; v += 3.141592 / 8.0) 
//        {
//            vec3 point = vec3(sin(u) * cos(v), sin(v), cos(u) * cos(v));
//            vec3 normal = normalize(point);
//            vec4 vertex = vec4(gs_in[0].Pos + point * gs_in[0].PointSize, 1.0);
//            FragPos = vec3(model * vertex);
//            gl_Position = projection * view * vertex;
//            Normal = mat3(transpose(inverse(model))) * normal;
//            EmitVertex();
//        }
//    }
//    EndPrimitive();
//
//    // for(int lat = 0; lat <= Sides / 2; ++lat) {
//    //     float latA = lat*pi/Sides;
//    //     float sinLatA = gs_in[0].PointSize * sin(latA);
//    //     float cosLatA = gs_in[0].PointSize * cos(latA);
//
//    //     for(int lon = 0; lon <= Sides + 1; ++lon) {
//    //         float lonA = lon*2.0*pi / Sides;
//    //         vec3 pos = vec3(cos(lonA) * cosLatA, sinLatA, -sin(lonA) * cosLatA);
//    //         vec3 normal = normalize(pos);
//    //         vec3 spherePt = gs_in[0].Pos + pos;
//    //         vec3 FragPt = vec3(model * vec4(spherePt, 1.0));
//    //         gl_Position = projection * view * vec4(FragPt, 1.0);
//
//    //         FragPos = FragPt;
//    //         Normal = mat3(transpose(inverse(model))) * normal;
//
//    //         EmitVertex();
//    //     }
//    // }
//}
//#endif
//
//#ifdef fragment_shader
//out vec4 FragColor;
//
//struct Light {
//    vec3 position;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    float shiness;
//};
//
//in vec3 FragPos;
//in vec3 Normal;
//
//in float Value;
//in vec3 material;
//flat in int isColor;
//flat in int hasValue;
//
//uniform sampler1D Texture;
//uniform int lightIsOn;
//uniform float minValue;
//uniform float maxValue;
//uniform int isEquivariance;//�Ƿ�Ȳ�(1Ϊ�Ȳ�,0Ϊ�ȱ�)
//uniform float textureCoordRatio;//���������ϵ��
//
//uniform vec3 viewPos;
//uniform Light light;
//
//vec3 calculateLightResult(vec3 norm, vec3 color, float AmbientOcclusion)
//{
//    if(lightIsOn == 0) return color;
//
//    vec3 viewDir = normalize(viewPos - FragPos);
//    if (dot(norm, viewDir) < 0.0) norm = -norm;
//    norm = normalize(norm);
//
//    vec3 ambient = light.ambient * color;//ambi
//    vec3 lightDir = normalize(light.position - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * diff * color * AmbientOcclusion;//diffu
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(norm, halfwayDir), 0.0), light.shiness);
//    vec3 specular = light.specular * spec * 0.3 * AmbientOcclusion;//specu
//    return ambient + diffuse + specular;
//}
//
//void main()
//{
//    float textCoord = isEquivariance == 1 ? (Value - minValue) / (maxValue - minValue) : log2(Value / minValue) / log2(maxValue / minValue);
//    textCoord *= textureCoordRatio;
//    vec3 color = texture(Texture, textCoord).rgb;
//
//    FragColor.rgb = calculateLightResult(Normal, color, 1.0);
//    FragColor.a = 1.0;
//} 
//
//#endif

#ifdef geometry_shader
layout(points) in;
layout(triangle_strip, max_vertices = 81) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in VS_OUT {
    float Value;
    flat int isColor;
    vec3 material;
    flat int hasValue;
    vec3 FragPos;
    vec3 Pos;
    int PointSize;
} gs_in[];

out vec3 Normal;
out vec3 FragPos;
flat out float Value;
flat out int isColor;
flat out int hasValue;
out vec3 material;

const float M_PI = 3.14159;

void main() 
{
    Value = gs_in[0].Value;
    isColor = gs_in[0].isColor;
    hasValue = gs_in[0].hasValue;
    material = gs_in[0].material;

    float px, py,pz;
    int i,j;
    float incO = 2*M_PI /5;
    float incA = M_PI /5;

    for (i= 0 ; i<= 5; i++)
    {
        for (j = 0; j<=5 ; j++) 
        {
            if (i % 2 == 0)
            {

            pz = cos (M_PI-(incA*j));
            py = sin (M_PI-(incA*j))*sin (incO*i);
            px = sin (M_PI-(incA*j))*cos (incO*i);

            vec3 position = vec3(px, py, pz);
            FragPos = vec3(model * vec4(position,1.0));
            Normal = mat3(transpose(inverse(model))) * normalize(position);
            gl_Position = projection * view * model * vec4(position*gs_in[0].PointSize + gs_in[0].Pos, 1.0);
            EmitVertex();

            pz = cos (M_PI-(incA*j));
            py = sin (M_PI-(incA*j))*sin (incO*(i+1));
            px = sin (M_PI-(incA*j))*cos (incO*(i+1));

            position = vec3(px, py, pz);       
            FragPos = vec3(model * vec4(position,1.0));
            Normal = mat3(transpose(inverse(model))) * normalize(position);
            gl_Position = projection * view * model * vec4(position*gs_in[0].PointSize + gs_in[0].Pos, 1.0);
            EmitVertex();
           }

        else 
            {
            pz = cos (incA*j);
            py = sin (M_PI-(incA*j))*sin (incO*i);
            px = sin (M_PI-(incA*j))*cos (incO*i);

            vec3 position = vec3(px, py, pz);       
            FragPos = vec3(model * vec4(position,1.0));
            Normal = mat3(transpose(inverse(model))) * normalize(position);
            gl_Position = projection * view * model * vec4(position*gs_in[0].PointSize + gs_in[0].Pos, 1.0);
            EmitVertex();

            pz = cos (incA*j);
            py = sin (incA*j)*sin (incO*(i+1));
            px = sin (incA*j)*cos (incO*(i+1));

            position = vec3(px, py, pz);       
            FragPos = vec3(model * vec4(position,1.0));
            Normal = mat3(transpose(inverse(model))) * normalize(position);
            gl_Position = projection * view * model * vec4(position*gs_in[0].PointSize + gs_in[0].Pos, 1.0);
            EmitVertex();
        
            }
        }
    }
    EndPrimitive();
 
}
#endif