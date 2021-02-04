#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

void FrameBufferSizeCallback(GLFWwindow *window, int w, int h);
void InputProcess(GLFWwindow *window);

int main(int argc, char *argv[])
{
	// 初始化glfw窗口
	glfwInit();
	
	// 指定OpenGL的主版本号和子版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(1280, 720, "GLFW WINDOW", NULL, NULL);
	if (window == NULL)
	{
		MessageBoxW(0, L"窗口创建失败", L"错误", MB_ICONERROR);
		glfwTerminate();
		return -1;
	}

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

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		InputProcess(window); // 处理输入事件

		// 清屏
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window); //交换缓存
		glfwPollEvents(); // 轮询事件
	}

	glfwTerminate();
	return 0;
}

/*
** 窗口大小更改处理回调
*/
void FrameBufferSizeCallback(GLFWwindow * window, int w, int h)
{
	glViewport(0, 0, w, h);
}

/*
** 处理输入事件
*/
void InputProcess(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
