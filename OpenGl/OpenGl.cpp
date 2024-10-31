#include "glad/glad.h"

#include "OpenGl.h"
#include "stb_image.h"


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

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

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


void OpenGl::Mainloop()
{
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	Progarm program1("../Shader/VertexShader.glsl", "../Shader/FragmentShader.glsl");

	program1.CreateProgram();

	uint32_t VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	Texture texture("../Shader/container.jpg");
	texture.Load();
	Texture texture2("../Shader/awesomeface.png", GL_RGBA);
	texture2.Load();

	program1.UseProgaram();
	program1.SetInt("ourTexture1", 0);
	program1.SetInt("ourTexture2", 1);

	int i = 0;
	while (!glfwWindowShouldClose(m_pWindow))
	{
		glClearColor(0.123f, 0.246f, 0.369f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ProcessInput(m_pWindow);
		glfwPollEvents();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	m_uShader = 0;
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
	shaderFile.close();
}

Progarm::Progarm(const std::string& strVertexShader, const std::string& strFragmentShader):
	m_uProgram(0)
{
	m_shaders.emplace_back(strVertexShader, GL_VERTEX_SHADER);
	m_shaders.emplace_back(strFragmentShader, GL_FRAGMENT_SHADER);
	for (auto& shader : m_shaders)
		shader.CreateShader();
}

void Progarm::AddShader(const std::string& shaderPath, uint32_t uShaderType)
{
	auto& shader = m_shaders.emplace_back(shaderPath, uShaderType);
	shader.CreateShader();
}

void Progarm::AddShader(Shader& shader)
{
	m_shaders.emplace_back(shader);
}

void Progarm::CreateProgram()
{
	m_uProgram = glCreateProgram();
	int i = 0;
	for (auto shader : m_shaders)
	{
		i++;
		glAttachShader(m_uProgram, shader.GetShader());
		int nShaders = 0;
		glGetProgramiv(m_uProgram, GL_ATTACHED_SHADERS, &nShaders);
		if (nShaders != i)
			std::cerr << "program attach shader fail" << std::endl;
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

void Progarm::SetBool(const std::string& strName, bool bValue)
{
	glUniform1i(glGetUniformLocation(m_uProgram, strName.c_str()), bValue);
}

void Progarm::SetInt(const std::string& strName, int32_t nValue)
{
	glUniform1i(glGetUniformLocation(m_uProgram, strName.c_str()), nValue);
}

void Progarm::SetFloat(const std::string& strName, float fValue)
{
	glUniform1i(glGetUniformLocation(m_uProgram, strName.c_str()), fValue);
}

uint32_t Texture::m_suActiveTexture = GL_TEXTURE0;

void Texture::Load()
{
	m_pData = stbi_load(m_stexturePath.c_str(), &m_nWidth, &m_nHeight, &m_nChannel, 0);
	if (!m_pData)
	{
		std::cerr << "load file " << m_stexturePath << " fail" << std::endl;
	}
	glGenTextures(1, &m_uTexture);
	glActiveTexture(m_suActiveTexture);
	m_suActiveTexture++;
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nWidth, m_nHeight, 0, m_uTextureType, GL_UNSIGNED_BYTE, m_pData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(m_pData);
}
