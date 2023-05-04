#version 330 core

out vec4 FragColor;

in vec3 v_Pos;
in vec4 v_Color;
in vec2 v_UV;
in float v_TexIndex;

uniform sampler2D SCREEN_TEXTURE;
uniform sampler2D u_Texture;

uniform float gTime;

void main() {

	vec2 newUV = vec2(v_Pos.x, v_Pos.y);

	vec4 tex1 = texture(SCREEN_TEXTURE, (newUV.xy+1)/2);
	vec4 resultColor = vec4(0.5 + 0.5 * cos(tex1.xyz*15 + gTime*3 + vec3(0, 2, 4)), tex1.a);

	FragColor = resultColor;
}