#version 330 core

out vec4 FragColor;

in vec3 v_Pos;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];
uniform vec4 u_Color;

void main() {

	int index = int(v_TexIndex);
	vec4 resultColor;

	if (index < 0) {
		resultColor = v_Color;
	}
	else {
		switch (index) {
		case 0:
			resultColor = texture(u_Textures[0], v_TexCoord);
			break;
		case 1:
			resultColor = texture(u_Textures[1], v_TexCoord);
			break;
		case 2:
			resultColor = texture(u_Textures[2], v_TexCoord);
			break;
		case 3:
			resultColor = texture(u_Textures[3], v_TexCoord);
			break;
		case 4:
			resultColor = texture(u_Textures[4], v_TexCoord);
			break;
		case 5:
			resultColor = texture(u_Textures[5], v_TexCoord);
			break;
		case 6:
			resultColor = texture(u_Textures[6], v_TexCoord);
			break;
		case 7:
			resultColor = texture(u_Textures[7], v_TexCoord);
			break;
		default:
			break;
		}
		
	}


	FragColor = resultColor;
}