#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;

flat out vec3 startPos;
out vec3 vertPos;

uniform mat4 u_ViewProjection;

void main()
{
	vec4 pos = u_ViewProjection * a_Position;
	gl_Position = pos;
	vertPos = pos.xyz / pos.w;
	startPos = vertPos;

}

#type fragment
#version 330 core

flat in vec3 startPos;
in vec3 vertPos;

layout(location = 0) out vec4 color;

uniform vec2  u_resolution;
uniform uint  u_pattern;
uniform float u_factor;
uniform vec4 u_Color;

void main()
{
	vec2  dir = (vertPos.xy - startPos.xy) * u_resolution / 2.0;
	float dist = length(dir);

	uint bit = uint(round(dist / u_factor)) & 15U;
	if ((u_pattern & (1U << bit)) == 0U)
		discard;
	color = u_Color; //vec4(1.0);
}