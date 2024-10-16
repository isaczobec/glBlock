#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <renderer.h>

#include <glm/vec4.hpp>

#include <fstream>
#include <string>
#include <sstream>

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








// --------------------------------------


int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 1280, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // init glew
    GLenum init_err = glewInit();
    if (init_err != GLEW_OK) {
        // we failed if not ok
        std::cout << "Error initializing glew!" << std::endl;
        return -1; 
    }

    std::cout << glGetString(GL_VERSION) << std::endl;


	// create input manager
	InputManager inputManager(window);

    // create camera
	renderer::Camera camera(45.0f, 1280.0f / 1280.0f, 0.1f, 100.0f);


	// create shader program
    ShaderProgramSource source = ParseShader("Shaders/BasicShader.shader");
    unsigned int shaderProgram = CreateShader(source.vertexSource, source.fragmentSource);
    unsigned int shaderProgram2 = CreateShader(source.vertexSource, source.fragmentSource);


    // create positions
    float positions[24] = {
         0.0f,0.0f,-50.0f,1.0f,
         1.0f,0.0f,-50.0f,1.0f,
         1.0f,1.0f,-50.0f,1.0f,
         0.0f,0.1f,-20.0f,1.0f,
         0.0f,1.1f,-20.0f,1.0f,
         1.0f,1.1f,-20.0f,1.0f,
    };

    float positions2[12] = {
       0.0f,0.0f,-3.0f,1.0f,
      -1.0f,1.0f,-3.0f,1.0f,
      -0.5f,0.0f,-3.0f,1.0f
    };

    // create renderer and some objects
    renderer::ObjectRenderer objectRender(&camera);

    // create example objects
    renderer::Object object1(0, 0, 0, positions, 6);
    object1.GenerateVertexBuffer();
    renderer::Object object2(0, 0, 0, positions2, 3);
    object2.GenerateVertexBuffer();

    object1.SetShaderProgram(shaderProgram);
    object2.SetShaderProgram(shaderProgram2);

    // add object
    objectRender.AddObject(&object1);
    objectRender.AddObject(&object2);





    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        objectRender.RenderAllObjects();

        //glDrawArrays(GL_TRIANGLES, 0, 6);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        inputManager.Refresh();
        /* Poll for and process events */
        glfwPollEvents();

		glm::vec2 movementInput = inputManager.GetMovementInput();
		glm::vec2 mouseDelta = inputManager.GetMousePositionDelta();
		camera.UpdateTransform(glm::vec3(movementInput.x, 0.0, movementInput.y), glm::vec3(mouseDelta.y * 0.01, mouseDelta.x * 0.01, 0.0));

        if (inputManager.GetMovementInput().y != 0 || inputManager.GetMovementInput().x != 0) {
			std::cout << "x: " << inputManager.GetMovementInput().x << "y: " << inputManager.GetMovementInput().y << std::endl;
        }

    }

    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram2);

    glfwTerminate();
    return 0;
}

