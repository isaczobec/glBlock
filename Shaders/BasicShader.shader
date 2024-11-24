#shader vertex
#version 440

layout(location = 0) in vec3 position;
out vec4 fragPosition;

// uniform mat4 u_projectionMatrix;

void main()
{
	vec4 pos = vec4(position,1.0);
   //gl_Position = u_projectionMatrix * pos;
   gl_Position = pos;
   fragPosition = pos;
};


#shader fragment
#version 440

in vec4 fragPosition;
layout(location = 0) out vec4 color;

void main()
{
   color = vec4(1,fragPosition.x,fragPosition.y,1);
};