#version 330 core

out vec4 FragColor;

in vec3 v_Pos;
in vec4 v_Color;
in vec2 v_UV;

vec4 circle(vec2 uv, vec3 pos, vec4 color) {
	float d = length(pos.xy - uv) - 1;
	float t = clamp(0.5, 0.0, 1.0);
	return vec4(color.rgb, color.a - t);
}

void main() {

	if (length(v_UV.xy - vec2(0.5)) < 0.2) {
		FragColor = v_Color;
	}
	else {
		FragColor = vec4(0., 0., 0., 0.);
	}
	//FragColor = circle(v_UV, v_Pos, v_Color);
}
