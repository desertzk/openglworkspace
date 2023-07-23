//#define GLFW_INCLUDE_VULKAN
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<windows.h>

#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<vector>

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


int main02(void)
{
    GLFWwindow* window;

    //DebugBreak();

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
    //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

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

    unsigned int m_VertexArray;
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id); //select buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    //use indices is to remove duplicate vertices
    unsigned int IndexBuffer;
    glGenBuffers(1, &IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//index: example I have three kind of attributes  position texture coordinate and normal.  position is index 0 texture coordinate index 1 normal index 2
    //size 2dimension is 2 3dimension is 3
    //stride how much you need to go forward to get second vertex if position is 3dimension 12 byte texture coordinate is 2dimension 8 byte normal is 3dimension 12 byte    0,12,20
    //pointer offset of next attribute

    //ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
    //unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    //glUseProgram(shader);
            // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);


    string vertexSrc = R"(
        #version 330 core

        layout(location = 0) in vec4 position;

        void main()
        {
	        gl_Position = position;
        };
    )";
    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        cout << infoLog.data();
        cout << "Vertex shader compilation failure!";
        return -1;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);



    string fragmentSrc = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        uniform vec4 u_Color;

        void main()
        {
	        color = vec4(0.2, 0.3, 0.8, 1.0);
	        //color = u_Color;
        };
)";
    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        cout << infoLog.data();
        cout << "Fragment shader compilation failure!";
        return -1;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    unsigned int  m_RendererID = glCreateProgram();
    GLuint program = m_RendererID;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        cout << "{0}" << infoLog.data();
        cout << "Shader link failure!";
        return -1;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //glBindVertexArray(m_VertexArray);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(vertexShader);
    glDeleteProgram(fragmentShader);
    glfwTerminate();
    return 0;
}