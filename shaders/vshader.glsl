#version 330 core

in vec4 vPosition;
in vec4 vColor; 

uniform mat4 ModelView;
uniform mat4 Projection;

out vec4 fColor; 

void main()
{
    gl_Position = Projection * ModelView * vPosition;
    fColor = vColor;
}
