#version 330 core

out vec4 FragColor;

in vec3 v_Pos;
in vec4 v_Color;

void main() {

	vec4 resultColor = v_Color;

	FragColor = resultColor;
}