#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
out vec4 fragPosition;

uniform mat4 u_projectionMatrix;

void main()
{
   gl_Position = u_projectionMatrix * position;
   fragPosition = position;
};


#shader fragment
#version 410 core

in vec4 fragPosition;
layout(location = 0) out vec4 color;

void main()
{
   color = vec4(1,fragPosition.x,fragPosition.y,1);
};