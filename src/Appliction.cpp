#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <renderer.h>

#include <glm/vec4.hpp>

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    // create positions
    float positions[12] = {
         0.0f,0.0f,0.0f,1.0f,
         1.0f,0.5f,0.0f,1.0f,
         0.5f,0.0f,0.0f,1.0f
    };

    float positions2[12] = {
       0.0f,0.0f,0.0f,1.0f,
      -1.0f,1.0f,0.0f,1.0f,
      -0.5f,0.0f,0.0f,1.0f
    };

    // create vertex buffer
  /*  unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);*/

    // create renderer and some objects
    renderer::ObjectRenderer objectRender;

    // create example objects
    renderer::Object object1(0, 0, 0, positions, 3);
    object1.GenerateVertexBuffer();
    renderer::Object object2(0, 0, 0, positions2, 3);
    object2.GenerateVertexBuffer();

    // add object
    objectRender.AddObject(&object1);
    objectRender.AddObject(&object2);

    std::string vertexShader = 
        "#version 410 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "out vec4 fragPosition;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "   fragPosition = position;\n"
        "}\n"
        ;
    std::string fragmentShader =
        "#version 410 core\n"
        "\n"
        "in vec4 fragPosition;\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(fragPosition.x,fragPosition.y,1,1);\n"
        "}\n"
        ;
    unsigned int shaderProgram = CreateShader(vertexShader, fragmentShader);

    objectRender.SetShaderProgram(shaderProgram);
    //glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        objectRender.RenderAllObjects();

        //glDrawArrays(GL_TRIANGLES, 0, 6);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




//#include <iostream>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <vector>
//
//#include <glm/vec4.hpp>
//
//static unsigned int CompileShader(unsigned int type, const std::string& source) {
//    unsigned int id = glCreateShader(type); // create the shader object
//    const char* src = source.c_str();
//    glShaderSource(id, 1, &src, nullptr); // add the source code to the shader object
//    glCompileShader(id); // compile the shader
//
//    // error handling
//    int result;
//    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//    if (result == GL_FALSE) {
//        int length;
//        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//        char* message = new char[length];
//        glGetShaderInfoLog(id, length, &length, message);
//        std::cout << "failed to compile shader!" << std::endl;
//        std::cout << message << std::endl;
//
//        glDeleteShader(id);
//        delete[] message;
//        return 0;
//    }
//
//    return id;
//}
//
//static unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader) {
//
//    unsigned int program = glCreateProgram();
//
//    // compile shaders
//    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//    // attach and link shaders
//    glAttachShader(program, vs);
//    glAttachShader(program, fs);
//    glLinkProgram(program);
//    glValidateProgram(program);
//
//    // delete shaders
//    glDeleteShader(vs);
//    glDeleteShader(fs);
//
//    return program;
//}
//
//
//
//
//
//
//
//
//// --------------------------------------
//
//
//int main(void)
//{
//
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    // init glew
//    GLenum init_err = glewInit();
//    if (init_err != GLEW_OK) {
//        // we failed if not ok
//        std::cout << "Error initializing glew!" << std::endl;
//        return -1;
//    }
//
//    std::cout << glGetString(GL_VERSION) << std::endl;
//
//    // create positions
//    float positions[6] = {
//        -1.0f,0.0f,
//         0.0f,1.0f,
//         0.5f,0.0f
//    };
//
//    // create vertex buffer
//    unsigned int buffer;
//    glGenBuffers(1, &buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, buffer);
//    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
//
//
//    std::string vertexShader =
//        "#version 410 core\n"
//        "\n"
//        "layout(location = 0) in vec4 position;\n"
//        "out vec4 fragPosition;\n"
//        "\n"
//        "void main()\n"
//        "{\n"
//        "   gl_Position = position;\n"
//        "   fragPosition = position;\n"
//        "}\n"
//        ;
//    std::string fragmentShader =
//        "#version 410 core\n"
//        "\n"
//        "in vec4 fragPosition;\n"
//        "layout(location = 0) out vec4 color;\n"
//        "\n"
//        "void main()\n"
//        "{\n"
//        "   color = vec4(fragPosition.x,fragPosition.y,0,1);\n"
//        "}\n"
//        ;
//    unsigned int shaderProgram = CreateShader(vertexShader, fragmentShader);
//    glUseProgram(shaderProgram);
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//        /* Render here */
//        glClear(GL_COLOR_BUFFER_BIT);
//
//
//
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}
