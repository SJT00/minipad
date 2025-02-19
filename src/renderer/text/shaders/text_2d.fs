#version 330 core

in VS_OUT{
    vec2 TexCoords;
    flat int index;
}fs_in;

out vec4 FragColor;

uniform sampler2DArray text;
uniform int letterMap[254];
uniform vec3 textColor;

void main()
{    
    float alpha = texture(text, vec3(fs_in.TexCoords, letterMap[fs_in.index])).r;
    FragColor = vec4(textColor, alpha);
}