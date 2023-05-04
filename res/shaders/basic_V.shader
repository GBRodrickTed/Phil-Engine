#version 330 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TexIndex;

out vec3 v_Pos;
out vec4 v_Color;
out vec2 v_UV;
out float v_TexIndex;

void main()
{
	gl_Position = vec4(a_Pos, 1.0f);

	v_Pos = gl_Position.xyz;
	v_Color = a_Color;
	v_UV = a_UV;
	v_TexIndex = a_TexIndex;
}