#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;

uniform mat4 u_ViewProjection;

void main()
{

	gl_Position = u_ViewProjection * a_Position;

};

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
};