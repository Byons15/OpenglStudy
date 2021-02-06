#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include <stb_image.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void FrameBufferSizeCallback(GLFWwindow *window, int w, int h);
void InputProcess(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float factor = 1.0f;

constexpr unsigned SCR_WIDTH = 1280;
constexpr unsigned SCR_HEIGHT = 720;

float deltaTime = 0;
float lastFrame = 0;
glm::vec3 cameraPos = { 0, 0, 4 };
glm::vec3 cameraFront = { 0, 0, -1 };
glm::vec3 cameraUp = { 0, 1, 0 };

int main(int argc, char *argv[])
{
	// 初始化glfw窗口
	glfwInit();
	
	// 指定OpenGL的主版本号和子版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW WINDOW", NULL, NULL);
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

	Shader shader("Ch01Ex03.vert", "Ch01Ex04.frag");

	//创建顶点缓存
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	unsigned indices[] = 
	{
		0, 1, 3,
		1, 2, 3,
	};

	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 顶点格式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//纹理坐标格式
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glActiveTexture(GL_TEXTURE0);
	// 绑定纹理
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置纹理包装和过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//翻转图片
	stbi_set_flip_vertically_on_load(true);

	//载入纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load("funny.jpg", &width, &height, &nrChannels, 0);
	GLint format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glActiveTexture(GL_TEXTURE1);

	// 绑定纹理
	unsigned texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 设置纹理包装和过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//载入纹理
	int width1, height1, nrChannels1;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data1 = stbi_load("container.jpg", &width1, &height1, &nrChannels1, 0);
	GLint format1 = (nrChannels1 == 3) ? GL_RGB : GL_RGBA;
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format1, width1, height1, 0, format1, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data1);

	shader.Use();
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	lastFrame = glfwGetTime();

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		float currTime = glfwGetTime();
		deltaTime = currTime - lastFrame;
		lastFrame = currTime;

		InputProcess(window); // 处理输入事件

		// 清屏
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		shader.Use();
		shader.SetFloat("factor", factor);

		// 观察变换矩阵
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// 投影变换矩阵
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		shader.SetMat4("view", glm::value_ptr(view));
		shader.SetMat4("projection", glm::value_ptr(projection));
		
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * glfwGetTime() * (i);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMat4("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		factor += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		factor -= 0.01f;
	}

	float cameraSpeed = 5.0f * deltaTime; //移动速度
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		// 叉乘Front和UP得到一个Right方向的向量并归一化，然后再乘上移动分量
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX, lastY;
	static float yaw = 0.0f, pitch = 0.0f;

	static bool firstMouse = true;
	if (firstMouse) {  //设置初始位置，防止突然跳到某个方向上
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = lastX - xpos;   //别忘了，在窗口中，左边的坐标小于右边的坐标，而我们需要一个正的角度
	float yoffset = lastY - ypos;   //同样，在窗口中，下面的坐标大于上面的坐标，而我们往上抬头的时候需要一个正的角度
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;  //旋转精度
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)  //往上看不能超过90度
		pitch = 89.0f;
	if (pitch < -89.0f)  //往下看也不能超过90度
		pitch = -89.0f;

	glm::vec3 front;
	front.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}
