#version 330 core

out vec4 FragColor;

in vec3 v_Pos;
in vec4 v_Color;
in vec2 v_UV;
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
			resultColor = texture(u_Textures[0], v_UV);
			break;
		case 1:
			resultColor = texture(u_Textures[1], v_UV);
			break;
		case 2:
			resultColor = texture(u_Textures[2], v_UV);
			break;
		case 3:
			resultColor = texture(u_Textures[3], v_UV);
			break;
		case 4:
			resultColor = texture(u_Textures[4], v_UV);
			break;
		case 5:
			resultColor = texture(u_Textures[5], v_UV);
			break;
		case 6:
			resultColor = texture(u_Textures[6], v_UV);
			break;
		case 7:
			resultColor = texture(u_Textures[7], v_UV);
			break;
		case 8:
			resultColor = texture(u_Textures[8], v_UV);
			break;
		case 9:
			resultColor = texture(u_Textures[9], v_UV);
			break;
		case 10:
			resultColor = texture(u_Textures[10], v_UV);
			break;
		case 11:
			resultColor = texture(u_Textures[11], v_UV);
			break;
		case 12:
			resultColor = texture(u_Textures[12], v_UV);
			break;
		case 13:
			resultColor = texture(u_Textures[13], v_UV);
			break;
		case 14:
			resultColor = texture(u_Textures[14], v_UV);
			break;
		case 15:
			resultColor = texture(u_Textures[15], v_UV);
			break;
		case 16:
			resultColor = texture(u_Textures[16], v_UV);
			break;
		case 17:
			resultColor = texture(u_Textures[17], v_UV);
			break;
		case 18:
			resultColor = texture(u_Textures[18], v_UV);
			break;
		case 19:
			resultColor = texture(u_Textures[19], v_UV);
			break;
		case 20:
			resultColor = texture(u_Textures[20], v_UV);
			break;
		case 21:
			resultColor = texture(u_Textures[21], v_UV);
			break;
		case 22:
			resultColor = texture(u_Textures[22], v_UV);
			break;
		case 23:
			resultColor = texture(u_Textures[23], v_UV);
			break;
		case 24:
			resultColor = texture(u_Textures[24], v_UV);
			break;
		case 25:
			resultColor = texture(u_Textures[25], v_UV);
			break;
		case 26:
			resultColor = texture(u_Textures[26], v_UV);
			break;
		case 27:
			resultColor = texture(u_Textures[27], v_UV);
			break;
		case 28:
			resultColor = texture(u_Textures[28], v_UV);
			break;
		case 29:
			resultColor = texture(u_Textures[29], v_UV);
			break;
		case 30:
			resultColor = texture(u_Textures[30], v_UV);
			break;
		case 31:
			resultColor = texture(u_Textures[31], v_UV);
			break;

		default:
			break;
		}
	}

	FragColor = resultColor;
}