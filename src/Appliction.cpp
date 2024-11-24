#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <renderer.h>

#include <glm/vec4.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <SOIL2.h>

#include<InputManager.h>

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

ShaderProgramSource ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::stringstream ss[2]; // init stringstream
    
    ShaderType currentShaderType = ShaderType::NONE;

    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                currentShaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                currentShaderType = ShaderType::FRAGMENT;
            }
        }
        else if (currentShaderType != ShaderType::NONE) {
            ss[(int)currentShaderType] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type); // create the shader object
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // add the source code to the shader object
    glCompileShader(id); // compile the shader

    // error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}

static unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader) {

    unsigned int program = glCreateProgram();

    // compile shaders
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach and link shaders
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // delete shaders
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}




// --------------------------------------


int main(void)
{


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const int WINDOW_SIZE = 1280;
    const int WINDOW_WIDTH = 1920;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	// set opengl version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(1280, 1280, "Hello World", NULL, NULL);
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); // set framebuffer size
	glViewport(0, 0, framebufferWidth, framebufferHeight); // set viewport size

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	glewExperimental = GL_TRUE;

    // init glew
    GLenum init_err = glewInit();
    if (init_err != GLEW_OK) {
        // we failed if not ok
        std::cout << "Error initializing glew!" << std::endl;
        glfwTerminate();
        return -1; 
    }

    // set opengl settings
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << glGetString(GL_VERSION) << std::endl;



    // create shader program 
	ShaderProgramSource source = ParseShader("Shaders/BasicShader.shader");
	unsigned int shaderProgram = CreateShader(source.vertexSource, source.fragmentSource);
	glUseProgram(shaderProgram);


    // create vertex arrays
    unsigned int testVertexArray;
	glGenVertexArrays(1, &testVertexArray);
	glBindVertexArray(testVertexArray);

    // create positions
    float positions[12] = {
         0.1f,0.1f,0.5f,
         0.9f,0.1f,0.5f,
         0.9f,0.9f,0.5f,
         0.1f,0.9f,0.5f,
    };

	unsigned int testVertexPosBuffer;
	glGenBuffers(1, &testVertexPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, testVertexPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);

    // create index buffer
	unsigned int indicies[6] = {
		0, 1, 2,
		2, 3, 0
	};
	unsigned int testIndexBuffer;
    glGenBuffers(1, &testIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {


        /* Render here */
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwPollEvents();

		glBindVertexArray(testVertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glFlush();
    }

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &testVertexArray);
	glDeleteBuffers(1, &testVertexPosBuffer);
	glDeleteBuffers(1, &testIndexBuffer);

    glfwTerminate();
    return 0;
}

