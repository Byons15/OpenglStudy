#include <Windows.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "../SDK/Shader.h"
#include <stb_image.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "glfw3.lib")

using namespace glm;

Camera camera;

float lastFrame = 0.0f;
float deltaTime = 0.0f;

void FrameBufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void InputProcess(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	deltaTime = glfwGetTime() - lastFrame;
	lastFrame += deltaTime;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "纹理加载失败，路径是:" << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
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

	float sensitivity = 0.5f;  //旋转精度
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)  //往上看不能超过90度
		pitch = 89.0f;
	if (pitch < -89.0f)  //往下看也不能超过90度
		pitch = -89.0f;

	camera.ProcessMouseMovement(xoffset, yoffset, pitch);
}

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

	glfwMakeContextCurrent(window); // 将窗口的上下文环境设置为主线程的上下文环境。

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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

	float vertices[] = {
		// 位置                  // 法线                 // 纹理坐标
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	Shader cubeShader("Light01.vert", "LightShader02.frag");
	Shader lampShader("Light01.vert", "LampShader.frag");

	// VBO，物体VAO
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 法向量属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// 光源VAO (VBO相同，因为顶点数据是同一组)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 位置属性（只需要更新跨度就可以了）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	mat4 cubeModel;
	cubeModel = translate(cubeModel, vec3(0, 0, 0));
	cubeModel = glm::rotate(cubeModel, glm::radians(10.0f), glm::vec3(1.0f, 1.0f, 0.0f));

	mat4 lampCubeModel;
	lampCubeModel = translate(lampCubeModel, vec3(1.2f, 1.0f, 2.0f));
	lampCubeModel = glm::scale(lampCubeModel, glm::vec3(0.2f));

	auto diffuseTex = loadTexture("diffuse_img.png");
	auto specularTex = loadTexture("specular.png");

	glEnable(GL_DEPTH_TEST);

	deltaTime = glfwGetTime();
	lastFrame = deltaTime;

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

	camera.Position = glm::vec3(0.0f, 0.0f, 4.0f);
	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		InputProcess(window); // 处理输入事件

		// 清屏
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 view = camera.GetViewMatrix();
		mat4 projection = perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		cubeShader.Use();
		glBindVertexArray(cubeVAO);
		cubeShader.SetVec3("light.direction", -0.2f, -1.0f, -0.3f);
		cubeShader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		cubeShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		cubeShader.SetMat4("view", glm::value_ptr(view));
		cubeShader.SetMat4("projection", glm::value_ptr(projection));
		cubeShader.SetVec3("viewPos", glm::value_ptr(camera.Position));

		cubeShader.SetInt("material.diffuse", 0);
		cubeShader.SetInt("material.specular", 1);
		cubeShader.SetFloat("material.shininess", 32.0f);

		cubeShader.SetVec3("pointLight.position", 1.2f, 1.0f, 2.0f);
		cubeShader.SetVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader.SetVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
		cubeShader.SetVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
		cubeShader.SetFloat("pointLight.constant", 1.0f);
		cubeShader.SetFloat("pointLight.linear", 0.09f);
		cubeShader.SetFloat("pointLight.quadratic", 0.032f);

		cubeShader.SetVec3("spotLight.position", glm::value_ptr(camera.Position));
		cubeShader.SetVec3("spotLight.direction", glm::value_ptr(camera.Front));
		cubeShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.SetVec3("spotLight.ambinet", 0, 0, 0);
		cubeShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		cubeShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		cubeShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

		for (int i = 0; i < 10; ++i)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * glfwGetTime() * (i);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader.SetMat4("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTex);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTex);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.Use();
		glBindVertexArray(lightVAO);
		lampShader.SetMat4("model", glm::value_ptr(lampCubeModel));
		lampShader.SetMat4("view", glm::value_ptr(view));
		lampShader.SetMat4("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents(); // 轮询事件
	}

	glfwTerminate();

	return 0;
}