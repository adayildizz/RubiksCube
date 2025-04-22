#version 410

in vec4 vPosition;
in vec4 vColor; // Rengi de alalım (isteğe bağlı ama iyi bir uygulama)

uniform mat4 ModelView;
uniform mat4 Projection;

out vec4 fColor; // Fragment shader'a rengi iletiyoruz

void main()
{
    gl_Position = Projection * ModelView * vPosition;
    fColor = vColor;
}
