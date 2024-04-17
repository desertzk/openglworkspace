//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include<string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader_m.h"
#include "shader_c.h"
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
    stbi_set_flip_vertically_on_load(true);  
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

    GLFWwindow* window = glfwCreateWindow(960, 540, "Compute Shader Demo", nullptr, nullptr);
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
    screenShader.use();
    //screenShader.setInt("texture1", carTexture);

    // framebuffer configuration
    // -------------------------
    // unsigned int framebuffer;
    // glGenFramebuffers(1, &framebuffer);
 //   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int ssbobuffer;
    glGenBuffers(1, &ssbobuffer);
    glBindBuffer(GL_COPY_WRITE_BUFFER, ssbobuffer);
    glBufferData(GL_COPY_WRITE_BUFFER, 2073600, NULL, GL_DYNAMIC_DRAW);
    void *ssbobuffer_addr = glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, 2073600, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
    //all 0 in src
    //87913 @1 memcpy(dest = 0x7fc312c46000, src = [binary data, size = 2025 kb], n = 2073600)
    memset(ssbobuffer_addr,0,2073600);
    glUnmapBuffer(GL_COPY_WRITE_BUFFER);
    



ComputeShader computeShader("res/shaders/computeShader.cs");



    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
 glBindTexture(GL_TEXTURE_2D, carTexture);	// use the color attachment texture as the texture of the quad plane
        // Set the time uniform variable
        screenShader.use();
        //screenShader.setInt("texture1", carTexture);
        glBindVertexArray(quadVAO);
       
        glDrawArrays(GL_TRIANGLES, 0, 6);


glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbobuffer);
computeShader.use();
glDispatchCompute(120, 68, 1);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLuint* ids = (GLuint*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER,
            0, 2073600, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);


    printf("ids :%d \n",ids[0]);

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    }





    // Clean up resources

    //glDeleteTextures(1, &texture);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}