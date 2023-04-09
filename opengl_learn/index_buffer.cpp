#define GLFW_INCLUDE_VULKAN
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "IndexBuffer.h"
#include "VertexBuffer.h"

using namespace std;

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}



static unsigned int CompiledShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();// &source[0]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//allocate on the stack
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compiled " <<
            (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

// bind vertexShader & fragmentShader and link them into a single shader program
// get a buffer return an ID 
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompiledShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompiledShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int mainib(void)
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

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "glewInit Error: %s\n", glewGetErrorString(err));

    }

    cout << "opengl version:" << glGetString(GL_VERSION) << endl;

    float positions[] = {
        -0.5f, -0.5f, //0
        0.5f, -0.5f,  //1
        0.5f, 0.5f,  //2
        -0.5f, 0.5f, //3
    };

    //use indices is to remove duplicate vertices
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    //unsigned int buffer_id;
    //glGenBuffers(1, &buffer_id);
    //glBindBuffer(GL_ARRAY_BUFFER, buffer_id); //select buffer
    //glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//index: example I have three kind of attributes  position texture coordinate and normal.  position is index 0 texture coordinate index 1 normal index 2
    //size 2dimension is 2 3dimension is 3
    //stride how much you need to go forward to get second vertex if position is 3dimension 12 byte texture coordinate is 2dimension 8 byte normal is 3dimension 12 byte    0,12,20
    //pointer offset of next attribute



    IndexBuffer ib(indices, 6);
    //unsigned int ibo;
    //glGenBuffers(1, &ibo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //select buffer
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);



    ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}