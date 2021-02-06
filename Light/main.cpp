#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "../SDK/Shader.h"
#include <stb_image.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void FrameBufferSizeCallback(GLFWwindow* window, int w, int h);
void InputProcess(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

constexpr unsigned SCR_WIDTH = 1280;
constexpr unsigned SCR_HEIGHT = 720;

int main(int argc, char* argv[])
{
	// 初始化glfw窗口
	glfwInit();

	// 指定OpenGL的主版本号和子版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW WINDOW", NULL, NULL);
	if (window == NULL)
	{
		MessageBoxW(0, L"窗口创建失败", L"错误", MB_ICONERROR);
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwMakeContextCurrent(window); // 将窗口的上下文环境设置为主线程的上下文环境。

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		MessageBoxW(0, L"初始化GLAD失败", L"错误", MB_ICONERROR);
		glfwTerminate();
		return -1;
	}

	// 注册窗口回调函数
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	glfwSwapInterval(1);

	return 0;
}