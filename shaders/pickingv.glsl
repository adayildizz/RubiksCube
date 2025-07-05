// pickingv.glsl
#version 330 core

uniform mat4 ModelView; // Changed from MVP
uniform mat4 Projection; // Added

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vPickingColor;

out vec4 fPickingColor;

void main() {
    gl_Position = Projection * ModelView * vPosition; // Combined here
    fPickingColor = vPickingColor;
}