//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include<string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader_m.h"
#include <filesystem>

using namespace std;
// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}




// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}



const char* computeShaderSource = R"(
#version 430
#extension GL_ARB_shading_language_420pack : require

#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
#if HLSLCC_ENABLE_UNIFORM_BUFFERS
#define UNITY_UNIFORM
#else
#define UNITY_UNIFORM uniform
#endif
#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
#if UNITY_SUPPORTS_UNIFORM_LOCATION
#define UNITY_LOCATION(x) layout(location = x)
#define UNITY_BINDING(x) layout(binding = x, std140)
#else
#define UNITY_LOCATION(x)
#define UNITY_BINDING(x) layout(std140)
#endif
precise vec4 u_xlat_precise_vec4;
precise ivec4 u_xlat_precise_ivec4;
precise bvec4 u_xlat_precise_bvec4;
precise uvec4 u_xlat_precise_uvec4;
UNITY_LOCATION(0) uniform  sampler2D FrameTex;
 struct rgbBuffer_type {
	uint[1] value;
};

layout(std430, binding = 0) buffer rgbBuffer {
	rgbBuffer_type rgbBuffer_buf[];
};
int u_xlati0;
uint u_xlatu0;
float u_xlat1;
uvec4 u_xlatu1;
bvec3 u_xlatb1;
vec3 u_xlat2;
ivec3 u_xlati2;
uvec3 u_xlatu2;
float u_xlat3;
bool u_xlatb3;
float u_xlat4;
float u_xlat6;
float u_xlat7;
bool u_xlatb7;
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
void main()
{
    u_xlatu0 = (uvec2(textureSize(FrameTex, 0)).x);
    u_xlatu1.xy = gl_GlobalInvocationID.xy;
    u_xlatu1.z = uint(0u);
    u_xlatu1.w = uint(0u);
    u_xlat2.xyz = texelFetch(FrameTex, ivec2(u_xlatu1.xy), int(u_xlatu1.w)).xyz;
    u_xlati0 = int(gl_GlobalInvocationID.y) * int(u_xlatu0) + int(gl_GlobalInvocationID.x);
    u_xlatb1.xyz = greaterThanEqual(vec4(0.0, 0.0, 0.0, 0.0), u_xlat2.xyzx).xyz;
    if(u_xlatb1.x){
        u_xlat1 = 0.0;
    } else {
        u_xlatb7 = 0.00313080009>=u_xlat2.x;
        if(u_xlatb7){
            u_xlat1 = u_xlat2.x * 12.9200001;
        } else {
            u_xlatb7 = u_xlat2.x<1.0;
            if(u_xlatb7){
                u_xlat7 = log2(u_xlat2.x);
                u_xlat7 = u_xlat7 * 0.416666687;
                u_xlat7 = exp2(u_xlat7);
                u_xlat1 = u_xlat7 * 1.05499995 + -0.0549999997;
            } else {
                u_xlat2.x = log2(u_xlat2.x);
                u_xlat2.x = u_xlat2.x * 0.454545438;
                u_xlat1 = exp2(u_xlat2.x);
            }
        }
    }
    if(u_xlatb1.y){
        u_xlat2.x = 0.0;
    } else {
        u_xlatb3 = 0.00313080009>=u_xlat2.y;
        if(u_xlatb3){
            u_xlat2.x = u_xlat2.y * 12.9200001;
        } else {
            u_xlatb3 = u_xlat2.y<1.0;
            if(u_xlatb3){
                u_xlat3 = log2(u_xlat2.y);
                u_xlat3 = u_xlat3 * 0.416666687;
                u_xlat3 = exp2(u_xlat3);
                u_xlat2.x = u_xlat3 * 1.05499995 + -0.0549999997;
            } else {
                u_xlat4 = log2(u_xlat2.y);
                u_xlat4 = u_xlat4 * 0.454545438;
                u_xlat2.x = exp2(u_xlat4);
            }
        }
    }
    if(u_xlatb1.z){
        u_xlat2.y = 0.0;
    } else {
        u_xlatb3 = 0.00313080009>=u_xlat2.z;
        if(u_xlatb3){
            u_xlat2.y = u_xlat2.z * 12.9200001;
        } else {
            u_xlatb3 = u_xlat2.z<1.0;
            if(u_xlatb3){
                u_xlat3 = log2(u_xlat2.z);
                u_xlat3 = u_xlat3 * 0.416666687;
                u_xlat3 = exp2(u_xlat3);
                u_xlat2.y = u_xlat3 * 1.05499995 + -0.0549999997;
            } else {
                u_xlat6 = log2(u_xlat2.z);
                u_xlat6 = u_xlat6 * 0.454545438;
                u_xlat2.y = exp2(u_xlat6);
            }
        }
    }
    u_xlat2.xy = u_xlat2.xy * vec2(255.0, 255.0);
    u_xlat2.xy = roundEven(u_xlat2.xy);
    u_xlat2.x = u_xlat2.x * 256.0;
    u_xlatu2.xy = uvec2(u_xlat2.xy);
    u_xlat6 = u_xlat1 * 255.0;
    u_xlat6 = roundEven(u_xlat6);
    u_xlat6 = u_xlat6 * 65536.0;
    u_xlatu2.z = uint(u_xlat6);
    u_xlati2.xz = ivec2(u_xlatu2.xz & uvec2(65280u, 16711680u));
    u_xlati2.x = u_xlati2.z + u_xlati2.x;
    u_xlati2.x = int(bitfieldInsert(u_xlati2.x, int(u_xlatu2.y), 0 & 0x1F, 8));
    rgbBuffer_buf[u_xlati0].value[(0 >> 2)] = uint(u_xlati2.x);
    return;
}

)";



const char* vertexShaderSource = R"(
    #version 430 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;
        
    out vec2 TexCoords;
        
    void main()
    {
        TexCoords = aTexCoords;
        gl_Position = vec4(aPos, 1.0);
    }
)";



const char* fragmentShaderSource = R"(
    #version 430 core
    out vec4 FragColor;
        
    in vec2 TexCoords;
        
    uniform sampler2D tex;
        
    void main()
    {             
        vec3 texCol = texture(tex, TexCoords).rgb;      
        FragColor = vec4(texCol, 1.0);
    }
)";

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
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
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


int main() {
    // Initialize GLFW and create a window
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "Compute Shader Demo", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }





    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


// Change the current working directory
std::filesystem::current_path("/root/umd/openglworkspace/opengl_learn/");
unsigned int carTexture = loadTexture("resources/textures/cartex.png");
Shader screenShader("res/shaders/5.1.base.vs", "res/shaders/5.1.base.fs");
    //screenShader.use();
    //screenShader.setInt("texture1", carTexture);

    // framebuffer configuration
    // -------------------------
    // unsigned int framebuffer;
    // glGenFramebuffers(1, &framebuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        glBindTexture(GL_TEXTURE_2D, carTexture);	// use the color attachment texture as the texture of the quad plane
        // Set the time uniform variable
        screenShader.use();
        //screenShader.setInt("texture1", carTexture);
        glBindVertexArray(quadVAO);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    // Create and compile the compute shader
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderSource, nullptr);
    glCompileShader(computeShader);
    checkCompileErrors(computeShader, "COMPUTE");

    // Create the compute shader program
    GLuint computeProgram = glCreateProgram();
    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);
    checkCompileErrors(computeProgram, "PROGRAM");


    // Get the uniform location for the time variable
    GLint timeLocation = glGetUniformLocation(computeProgram, "time");

// texture size
const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;

unsigned int texture;

glGenTextures(1, &texture);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, 
             GL_FLOAT, NULL);

glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);





//quad shader
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexShaderSource, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        unsigned int quadID = glCreateProgram();
        glAttachShader(quadID, vertex);
        glAttachShader(quadID, fragment);
        glLinkProgram(quadID);
        checkCompileErrors(quadID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);















    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Set the time uniform variable
        float time = static_cast<float>(glfwGetTime());
        glUniform1f(timeLocation, time);

       // Use the compute shader program
    glUseProgram(computeProgram);
    glDispatchCompute((unsigned int)TEXTURE_WIDTH, (unsigned int)TEXTURE_HEIGHT, 1);

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);



    // render image to quad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(quadID); 
    glUniform1i(glGetUniformLocation(quadID, "tex"), 0); 
    //screenQuad.setInt("tex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderQuad();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteProgram(computeProgram);
    glDeleteShader(computeShader);
    glDeleteTextures(1, &texture);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}