#include <cstdio>
#include <cstdlib>
#include <shaders.hpp>

#include <shadersSources.hpp>

const char* computeVertexShaderSource   = COMPUTE_VERTEX_SHADER_SOURCE;
const char* computeFragmentShaderSource = COMPUTE_FRAGMENT_SHADER_SOURCE;
const char* renderVertexShaderSource    = RENDER_VERTEX_SHADER_SOURCE;
const char* renderFragmentShaderSource  = RENDER_FRAGMENT_SHADER_SOURCE;

GLuint Shaders::linkShaderProgram(const char* vertexSource, const char* fragmentSource) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

Shaders::Shaders(GLuint width, GLuint height) {
	// Replace %f in COMPUTE_VERTEX_SHADER_SOURCE with a float
	// Will look like 0.123456 (6 decimal places)
	// 8 characters - 2 characters (%f) = 6 characters
	char* computeFragmentShaderSourceForReal = (char*)malloc(sizeof(COMPUTE_FRAGMENT_SHADER_SOURCE) + 6);
	sprintf(
		computeFragmentShaderSourceForReal,
		computeFragmentShaderSource,
		1.0/width,
		1.0/height
	);

	computeShaderProgram = linkShaderProgram(
		computeVertexShaderSource,
		computeFragmentShaderSourceForReal
	);

	renderShaderProgram = linkShaderProgram(
		RENDER_VERTEX_SHADER_SOURCE,
		RENDER_FRAGMENT_SHADER_SOURCE
	);

	posXLocation = glGetUniformLocation(renderShaderProgram, "uPosX");
	posYLocation = glGetUniformLocation(renderShaderProgram, "uPosY");
	zoomLocation = glGetUniformLocation(renderShaderProgram, "uZoom");
}
