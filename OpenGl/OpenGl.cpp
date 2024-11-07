#include "glad/glad.h"
#include "OpenGl.h"

#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "stb_image.h"


const int cnWidth = 800;
const int cnHeight = 600;

Camera camera;


static void framBufferSizeCallBack(GLFWwindow *pWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* pWindows, double x, double y)
{
	static float s_flastx = x, s_flasty = y;

	float foffsetX = x - s_flastx;
	float foffsetY = s_flasty - y;
	s_flastx = x;
	s_flasty = y;
	camera.ProcessMouseInput(foffsetX, foffsetY);
}

void scroll_callback(GLFWwindow* pWindow, double doffsetX, double doffsetY)
{

}

static void ProcessInput(GLFWwindow* pWindow, Camera& camera)
{
	static float s_flastTim = glfwGetTime();
	float fCurrentTime = glfwGetTime();
	float fDeltaTime = fCurrentTime - s_flastTim;
	auto keyState = glfwGetKey(pWindow, GLFW_KEY_ESCAPE);
	if (keyState == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GLFW_TRUE);


	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyBordInput(InputDirection::UP, fDeltaTime);

	if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyBordInput(InputDirection::DOWN, fDeltaTime);

	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyBordInput(InputDirection::LEFT, fDeltaTime);

	if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyBordInput(InputDirection::RIGHT, fDeltaTime);

	s_flastTim = fCurrentTime;
}

void OpenGl::run()
{
	Init();
	Mainloop();
}

void OpenGl::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow = glfwCreateWindow(cnWidth, cnHeight, "OpenGl", nullptr, nullptr);
	if (!m_pWindow)
		throw std::runtime_error("Create glfw Window fail");

	glfwMakeContextCurrent(m_pWindow);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
		throw std::runtime_error("init glad fail");

	glViewport(0, 0, cnWidth, cnHeight);

	glfwSetFramebufferSizeCallback(m_pWindow, framBufferSizeCallBack);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(m_pWindow, mouse_callback);

}
/*Default
void OpenGl::Mainloop()
{

	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};


	Progarm program;
	program.AddShader("../Shader/VertexShader.glsl", GL_VERTEX_SHADER);
	program.AddShader("../Shader/FragmentShader.glsl", GL_FRAGMENT_SHADER);

	uint32_t VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	program.CreateProgram();

	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.123f, 0.246f, 0.369f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ProcessInput(m_pWindow);
		glfwPollEvents();
		program.UseProgaram();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(m_pWindow);
	}
}
*/

/*第一章
void OpenGl::Mainloop()
{

	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角

		0.5f, 0.5f, 0.0f,   // 右上角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};

	Shader fragmentShader("../Shader/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	Shader vertexShader("../Shader/VertexShader.glsl", GL_VERTEX_SHADER);
	Shader fragmentShader1("../Shader/FragmentShader1.glsl", GL_FRAGMENT_SHADER);

	fragmentShader.CreateShader();
	vertexShader.CreateShader();
	fragmentShader1.CreateShader();

	Progarm program1, program2;
	program1.AddShader(vertexShader);
	program1.AddShader(fragmentShader);
	
	program2.AddShader(vertexShader);
	program2.AddShader(fragmentShader1);
	
	uint32_t VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);
	glGenBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / 2, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / 2, vertices + 9, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	program1.CreateProgram();
	program2.CreateProgram();
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.123f, 0.246f, 0.369f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ProcessInput(m_pWindow);
		glfwPollEvents();
		program1.UseProgaram();
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		program2.UseProgaram();
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(m_pWindow);
	}
}
*/

/*相机
void OpenGl::Mainloop()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	int indices[] = {
		0, 1, 2,
		0, 2, 3
	};


	std::vector<glm::vec3> cubePositions = {
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

	Progarm program1("../Shader/VertexShader.glsl", "../Shader/FragmentShader.glsl");

	program1.CreateProgram();

	uint32_t VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	Texture texture("../Shader/container.jpg");
	texture.Load();
	Texture texture2("../Shader/awesomeface.png", GL_RGBA);
	texture2.Load();
	
	glEnable(GL_DEPTH_TEST);
	int i = 0;

	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.123f, 0.246f, 0.369f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ProcessInput(m_pWindow, camera);
		glfwPollEvents();
		float angle = 0.05f * i;
		program1.UseProgaram();
		program1.SetInt("ourTexture1", 0);
		program1.SetInt("ourTexture2", 1);
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(45.0f), float(cnWidth) / cnHeight, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(program1.GetProgram(), "trans"), 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(program1.GetProgram(), "view"), 1, false, glm::value_ptr(camera.GetView()));
		
		glBindVertexArray(VAO);

		for (auto& position : cubePositions)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, position);
			
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			glUniformMatrix4fv(glGetUniformLocation(program1.GetProgram(), "model"), 1, false, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(m_pWindow);
		i++;
	}
}
*/

void OpenGl::Mainloop()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	Progarm program1("../Shader/lightVertex.glsl", "../Shader/lightFragment.glsl");
	Progarm program2("../Shader/lightsource.glsl", "../Shader/lightSourceFragment.glsl");
	program1.CreateProgram();
	program2.CreateProgram();

	uint32_t VBO, VAO, ulightVAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &ulightVAO);
	glBindVertexArray(ulightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	Texture texture("../Shader/container.jpg");
	texture.Load();
	Texture texture2("../Shader/awesomeface.png", GL_RGBA);
	texture2.Load();

	glEnable(GL_DEPTH_TEST);
	int i = 0;
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ProcessInput(m_pWindow, camera);
		glfwPollEvents();
		float angle = 0.05f * i;
		program1.UseProgaram();
		program1.SetVec3("light", glm::vec3(1.0f, 1.0f, 1.0f));
		program1.SetVec3("color", glm::vec3(1.0f, 0.5f, 0.1f));
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(45.0f), float(cnWidth) / cnHeight, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(program1.GetProgram(), "trans"), 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(program1.GetProgram(), "view"), 1, false, glm::value_ptr(camera.GetView()));

		glBindVertexArray(VAO);
		
		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(program1.GetProgram(), "model"), 1, false, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		program2.UseProgaram();
		glUniformMatrix4fv(glGetUniformLocation(program2.GetProgram(), "trans"), 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(program2.GetProgram(), "view"), 1, false, glm::value_ptr(camera.GetView()));

		glBindVertexArray(ulightVAO);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
		model = glm::translate(model, glm::vec3(1.2, 1.0, 2.0));
		
		glUniformMatrix4fv(glGetUniformLocation(program2.GetProgram(), "model"), 1, false, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(m_pWindow);
		i++;
	}
}

void OpenGl::UnInit()
{
	glfwTerminate();
}

