#include "glad/glad.h"

#include "OpenGl.h"


const int cnWidth = 800;
const int cnHeight = 600;

static void framBufferSizeCallBack(GLFWwindow *pWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void ProcessInput(GLFWwindow* pWindow)
{
	auto keyState = glfwGetKey(pWindow, GLFW_KEY_ESCAPE);
	if (keyState == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
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
}

void OpenGl::Mainloop()
{

	float vertices[] = {
		-0.5, 0, 0,
		0.5, 0, 0,
		0, 0.5, 0 
	};


	Progarm program;
	program.AddShader("../Shader/VertexShader.glsl", GL_VERTEX_SHADER);
	program.AddShader("../Shader/FragmentShader.glsl", GL_FRAGMENT_SHADER);

	program.CreateProgram();

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.123f, 0.246f, 0.369f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ProcessInput(m_pWindow);
		glfwPollEvents();
		program.UseProgaram();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(m_pWindow);
	}
}

void OpenGl::UnInit()
{
	glfwTerminate();
}

Shader::Shader(const std::string& shaderPath, uint32_t uShaderType)
{
	m_shaderFilePath = shaderPath;
	m_uShaderType = uShaderType;
}

Shader::~Shader()
{
	if (m_uShader)
		glDeleteShader(m_uShader);
}

static uint32_t GetFileSize(FILE* pFile)
{
	uint32_t uFileSize = 0;
	if (!pFile)
		return uFileSize;
	fseek(pFile, 0, SEEK_END);
	uFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	return uFileSize;
}

template <class T>
static uint32_t GetFileSize(T& rFile)
{
	uint32_t uFileSize = 0;
	if (!rFile.is_open())
		return uFileSize;
	rFile.seekg(0, std::ios::end);
	uFileSize = rFile.tellg();
	rFile.seekg(0, std::ios::beg);
	return uFileSize;
}

void Shader::CreateShader()
{
	if (!std::filesystem::exists(m_shaderFilePath))
		throw std::runtime_error("shader file not exists");
	
	std::ifstream shaderFile(m_shaderFilePath);	
	if (!shaderFile.is_open())
		throw std::runtime_error("shader file is not open");

	uint32_t uShaderFileSize = GetFileSize(shaderFile);
	
	char* strShader = new char[uShaderFileSize];
	memset(strShader, 0, uShaderFileSize);

	shaderFile.read(strShader, uShaderFileSize);
	m_uShader = glCreateShader(m_uShaderType);
	glShaderSource(m_uShader, 1, &strShader, NULL);
	glCompileShader(m_uShader);
	int success = false;
	glGetShaderiv(m_uShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorBuffer[512] = { 0 };
		glGetShaderInfoLog(m_uShader, 512, NULL, errorBuffer);
		std::cerr << "complied shadder error " << errorBuffer << std::endl;
	}

	delete[] strShader;
}

void Progarm::AddShader(const std::string& shaderPath, uint32_t uShaderType)
{
	auto shader = m_shaders.emplace_back(shaderPath, uShaderType);
	shader.CreateShader();
}

void Progarm::CreateProgram()
{
	m_uProgram = glCreateProgram();

	for (auto shader : m_shaders)
	{
		glAttachShader(m_uProgram, shader.GetShader());
	}

	glLinkProgram(m_uProgram);
	int success = 0;
	glGetProgramiv(m_uProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[512] = { 0 };
		glGetProgramInfoLog(m_uProgram, 512, NULL, errorLog);
		std::cerr << "Link program fail, error is " << errorLog << std::endl;
		throw std::runtime_error("Link program fail");
	}
}

void Progarm::UseProgaram()
{
	glUseProgram(m_uProgram);
	//glGetProgramiv(m_uProgram, )
}
