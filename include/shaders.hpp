#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <glad/glad.h>

class Shaders {
	private:
		GLuint linkShaderProgram(const char* vertexSource, const char* fragmentSource);

	public:
		GLuint computeShaderProgram;
		GLuint renderShaderProgram;

		GLint posXLocation;
		GLint posYLocation;
		GLint zoomLocation;

		Shaders(GLuint width, GLuint height);
};

#endif
