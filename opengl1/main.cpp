#include "libs.h"
#include "config.h"

bool loadShaders(GLuint &program)
{
	bool loadSuccess = true;

	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	// Vertex Shader

	in_file.open("test_vertex.glsl");

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "Error | Vertex shader could not be loaded";
		loadSuccess = false;
	}

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();

	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		
		std::cout << "Error | Vertex shader could not be compiled";
		std::cout << infoLog << "\n";

		loadSuccess = false;
	}

	temp = "";
	src = "";

	// Fragment Shader

	in_file.open("test_fragment.glsl");

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "Error | Fragment shader could not be loaded";
		loadSuccess = false;
	}

	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();

	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

		std::cout << "Error | Fragment shader could not be compiled";
		std::cout << infoLog << "\n";

		loadSuccess = false;
	}

	// Program

	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);

		std::cout << "Error | Program could not be linked";
		std::cout << infoLog << "\n";

		loadSuccess = false;
	}

	// End of Shader Loading

	glUseProgram(0);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
}

int main()
{
	glfwInit();

	const int WINDOW_WIDTH = 512;
	const int WINDOW_HEIGHT = 512;

	const char* WINDOW_TITLE = "My Window";

	int frameBufferWidth = 0;
	int frameBufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error | GLEW32 Could not be initilised" << std::endl;
		glfwTerminate();
	}

	// OpenGL Options

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glFrontFace(GL_CCW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Shaders

	GLuint core_program;

	if (!loadShaders(core_program))
	{
		glfwTerminate();
	}

	// Game Loop

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	
	
		glClearColor(.0f, .0f, .0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glfwSwapBuffers(window);
		glFlush();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	glDeleteProgram(core_program);

	return 0;
}
