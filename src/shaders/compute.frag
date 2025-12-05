#version 330 core
in vec2 texCoords;

uniform sampler2D uTexture;

out vec4 fragColor;

void main() {
	// Offsets, %f are replaced with floats at runtime
	float ox = %f;
	float oy = %f;

	float e  = texture(uTexture, vec2(texCoords.x + ox, texCoords.y     )).r;
	float ne = texture(uTexture, vec2(texCoords.x + ox, texCoords.y + oy)).r;
	float n  = texture(uTexture, vec2(texCoords.x     , texCoords.y + oy)).r;
	float nw = texture(uTexture, vec2(texCoords.x - ox, texCoords.y + oy)).r;
	float w  = texture(uTexture, vec2(texCoords.x - ox, texCoords.y     )).r;
	float sw = texture(uTexture, vec2(texCoords.x - ox, texCoords.y - oy)).r;
	float s  = texture(uTexture, vec2(texCoords.x     , texCoords.y - oy)).r;
	float se = texture(uTexture, vec2(texCoords.x + ox, texCoords.y - oy)).r;

	float neighbours = e + ne + n + nw + w + sw + s + se;

	float currentColor = texture(uTexture, texCoords).r;
	float newColor = 0.0;

	if (currentColor == 0.0) {
		if (neighbours == 3.0) {
			newColor = 1.0;
		}
	} else {
		if (neighbours == 2.0 || neighbours == 3.0) {
			newColor = 1.0;
		} else {
			newColor = 0.0;
		}
	}

	fragColor = vec4(newColor, newColor, newColor, 1.0);
}
