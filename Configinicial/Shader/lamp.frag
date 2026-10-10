
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 lampColor;
uniform bool useTexture;
uniform sampler2D texture_diffuse1;

void main()
{
    if (useTexture)
        FragColor = texture(texture_diffuse1, TexCoords);
    else
        FragColor = vec4(lampColor, 1.0);
}
