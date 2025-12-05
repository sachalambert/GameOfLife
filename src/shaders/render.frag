#version 330 core
in vec2 texCoords;

uniform float uPosX;
uniform float uPosY;

uniform float uZoom;
//uniform vec2 uZoomCenter;

uniform sampler2D uTexture;

out vec4 fragColor;

void main() {
	//vec2 uZoomCenter = vec2(0.5)
	//vec2 coords = vec2(texCoords.x + uPosX, texCoords.y + uPosY) * uZoom;
	/*vec2 coords = vec2(
		(texCoords.x + uPosX)*uZoom,
		(texCoords.y + uPosY)*uZoom
	);*/
	vec2 coords = vec2(
		(texCoords.x - 0.5)*uZoom + 0.5,
		(texCoords.y - 0.5)*uZoom + 0.5
	);

	coords += vec2(uPosX, uPosY);

	fragColor = texture(uTexture, coords);
}
