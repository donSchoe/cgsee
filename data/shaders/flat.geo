#version 150 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 _normal[3];
out vec3 normal;

in vec3 _position[3];
out vec3 position;

in vec2 b_texc[3];
out vec2 texc;

void main(void)
{
    normal=normalize( (_normal[0] + _normal[1] + _normal[2])/3 );

    position=vec3(0.0);
    gl_Position=vec4(0.0);

    for(int i = 0; i < 3; i++) {
        position = _position[i];
        gl_Position=gl_in[i].gl_Position;;
        texc = b_texc[i];
        EmitVertex();
    }

    EndPrimitive();
}
