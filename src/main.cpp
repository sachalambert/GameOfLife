#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shaders.hpp>

#include <iostream>

int windowWidth, windowHeight; // TODO
float posX = 0.0f, posY = 0.0f;
float zoom = 1.0f;

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
}

bool controlKeyPressed = false;
void keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
	if (key == GLFW_KEY_A) {
		glfwSetWindowShouldClose(window, 1);
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		zoom = 1.0f;
	}

	if (key == GLFW_KEY_LEFT_CONTROL) {
		controlKeyPressed = (action == GLFW_PRESS);
	}

	if (key == GLFW_KEY_V) {
		glfwSwapInterval(action == GLFW_RELEASE);
	}

	if (action == GLFW_RELEASE) return;
	switch (key) {
		case GLFW_KEY_RIGHT:
			posX += 1.0f;
			break;
		case GLFW_KEY_UP:
			posY += 1.0f;
			break;
		case GLFW_KEY_LEFT:
			posX -= 1.0f;
			break;
		case GLFW_KEY_DOWN:
			posY -= 1.0f;
			break;
		default:
			break;
	}
}

bool dragging = false;
void mouseButtonCallback([[maybe_unused]] GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		dragging = action == GLFW_PRESS;
	}
}

void scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xOffset, double yOffset) {
	if (yOffset < 0) {
		zoom *= 1.1f;
	} else if (yOffset > 0) {
		zoom /= 1.1f;
	}
}

void cursorPosCallback([[maybe_unused]] GLFWwindow* window, double xPos, double yPos) {
	static double xPosPrev = 0.0f;
	static double yPosPrev = 0.0f;

	if (dragging) {
		posX -= (xPos - xPosPrev) / windowWidth * zoom;
		posY += (yPos - yPosPrev) / windowHeight * zoom;
	}

	xPosPrev = xPos;
	yPosPrev = yPos;
}


int main(int argc, char* argv[]) {
	GLsizei width = -1;
	GLsizei height = -1;

	if (argc > 1) {
		width = std::stoi(argv[1]);
	} else {
		width = 100;
	}
	if (argc > 2) {
		height = std::stoi(argv[2]);
	} else {
		height = width;
	}

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(500, 500, "TEST", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window.\n";
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD.\n";
		return 1;
	}

	framebufferSizeCallback(window, 500, 500);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	float vertices[] = {
		-1.0f, -1.0f, // Bottom left
		 1.0f, -1.0f,	// Bottom right
		-1.0f,  1.0f, // Top left
		 1.0f, -1.0f,	// Bottom right
		 1.0f,  1.0f, // Top right
		-1.0f,  1.0f  // Top left
	};

	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao);

	Shaders shaders = Shaders(width, height);

	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	// Fill texture1 with random pixels
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);*/

	GLubyte* data = (GLubyte*)malloc(sizeof(GLubyte) * width*height*3);

	for (GLsizei i = 0; i < width*height; i++) {
		int v = (rand() < (RAND_MAX/2)) * 255;
		data[3*i + 0] = v;
		data[3*i + 1] = v;
		data[3*i + 2] = v;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	// Only allocate memory for texture2
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);*/

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	// Generate framebuffer
	GLuint fbo;
	glGenFramebuffers(1, &fbo);

	while (!glfwWindowShouldClose(window)) {
		// Update cells
		glUseProgram(shaders.computeShaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2, 0);
		glViewport(0, 0, width, height);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Actual render
		glUseProgram(shaders.renderShaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUniform1f(shaders.posXLocation, posX);
		glUniform1f(shaders.posYLocation, posY);
		glUniform1f(shaders.zoomLocation, zoom);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Swap textures
		GLuint temp = texture1;
		texture1 = texture2;
		texture2 = temp;

		// End
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
