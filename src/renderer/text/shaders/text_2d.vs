#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoords;

out VS_OUT{
    vec2 TexCoords;
    flat int index;
}vs_out;

uniform mat4 transforms[254];
uniform mat4 projection;

void main()
{
    gl_Position = projection *transforms[gl_InstanceID]* vec4(vertex, 0.0, 1.0);
    vs_out.TexCoords = texCoords;
    vs_out.index=gl_InstanceID;
}