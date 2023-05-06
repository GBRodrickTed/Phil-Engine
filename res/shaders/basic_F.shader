#version 330 core

out vec4 FragColor;

in vec3 v_Pos;
in vec4 v_Color;
in vec2 v_UV;
in float v_TexIndex;

uniform sampler2D SCREEN_TEXTURE;
uniform sampler2D u_Texture;

uniform float gTime;

vec2 SineWave(vec2 p)
{
    // convert Vertex position <-1,+1> to texture coordinate <0,1> and some shrinking so the effect dont overlap screen
	float ty = 1 + gTime;
	float tx = 1 + gTime;
    p.x = (0.55 * p.x) + 0.5;
    p.y = (-0.55 * p.y) + 0.5;
    // wave distortion
    float x = sin(25.0 * p.y + 30.0 * p.x + 6.28 * tx) * 0.05;
    float y = sin(25.0 * p.y + 30.0 * p.x + 6.28 * ty) * 0.05;
    return vec2(p.x + x, p.y + y);
}

void main() {

	//vec2 newUV = vec2(v_Pos.x, v_Pos.y);

	//vec2 betterUV = vec2((newUV.xy + 1) / 2);

	//vec4 tex_scr = texture(SCREEN_TEXTURE, (newUV.xy + 1) / 2);
	//vec4 tex1 = texture(u_Texture, v_UV);

	//float texMix = dot(tex1, tex_scr);
	////vec4(0.5 + 0.5 * cos(tex1.xyz*15 + gTime*3 + vec3(0, 2, 4)), tex1.a);
	////vec4 resultColor = vec4(0.5 + 0.5 * cos(tex1.xyz * 15 + gTime * 3 + vec3(0, 2, 4)), tex1.a) * texMix;

	//vec3 greyScale = vec3(0.3, 0.59, 0.11);
	//vec4 tempColor = vec4(vec3(dot(tex_scr.rgb, greyScale)), tex_scr.a);

	//vec4 resultColor = tempColor;

	//FragColor = resultColor;

	FragColor = texture(SCREEN_TEXTURE, SineWave(v_UV));
}
