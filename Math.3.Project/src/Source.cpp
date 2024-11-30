#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* win, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float  lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
float deltaTime = 0.0f, lastFrame = 0.0f, currentFrame = 0.0f;
int val = 1;
bool firstEntry = true;
Camera myCamera(glm::vec3(1.0f, 0.5f, 4.0f));
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LinearAlg", nullptr, nullptr);
	if (!window) {
		std::cout << "Error!Window Couldn't be Created!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Error!OpenGL functions Couldn't be Loaded!\n";
		glfwTerminate();
		return -1;
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);


	float vertices[]{
	//  X   , Y   , Z
		0.0f, 0.0f, 0.0f, //1.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, //1.0f, 0.0f, 1.0f,
		
		1.0f, 1.0f, 0.0f//, 1.0f, 0.0f, 0.0f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// create a buffer on the gpu(vram)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// allocate the size of the buffer and send the data of that buffer and set the nature of the access of that data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	Shader sh("C:/dev/Math.3.Project/Math.3.Project/resources/shaders/vertexShader.shader", "C:/dev/Math.3.Project/Math.3.Project/resources/shaders/fragmentShader.shader");

	sh.use();
	glViewport(0, 0, 800, 600);


	glm::mat4 model = glm::mat4(1.0f);
	// in that test case the resulting transformation is the same but generally it is not!
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	// u = (-0.5f, 0.0f, 0.0f), a = (1.0f, -1.0f, 1.0f), a * u = (-0.5, 0.0f, 0.0f) = u
	model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
	
	// model * vec = Identity * translate * scale * vec
	// model = trans * model; this doesn't happen!



	glm::mat4 rot = glm::mat4(1.0f);
	// rotates around an axis (in 2d it is always the z axis ) with a given angle (radians);
	rot = glm::rotate(rot, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	
	
	int flip = 1;
	while (!glfwWindowShouldClose(window)) {
		// process input

		processInput(window);
		// set the color of the background of the window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		// render

		glm::mat4 rotating = glm::mat4(1.0f);

		
		
		float t = glfwGetTime();

		if (int(t) % 720 == 0)
			flip = flip == 1 ? -1 : 1;
		

		rotating = glm::rotate(rotating, glm::radians(flip * 30 * t), glm::vec3(0.0f, 0.0f, 1.0f));
		
		sh.setMatrix4f("model", false, glm::value_ptr(rotating));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* win, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		myCamera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		myCamera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		myCamera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		myCamera.processKeyboard(RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {

	if (firstEntry) {
		firstEntry = false;
		lastX = xPos, lastY = yPos;
	}

	float xOffset = xPos - lastX, yOffset = lastY - yPos;//yPos - lastY;//
	lastX = xPos, lastY = yPos;
	myCamera.processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {

	myCamera.processMouseScroll(yOffset);
}