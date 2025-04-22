#version 410
in vec4 fColor; // Köşe shader'ından gelen renk

out vec4 color;

void main()
{
    color = fColor;
}
