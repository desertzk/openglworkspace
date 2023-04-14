#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // stringstream
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            // �� type ��Ϊ string stream �� index��������
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) // �ڶ�����������Ϊ GLenum �� unsigned int ��
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // ���ص���һ��ָ�� string �ڲ���ָ�룬������ string ������ڲ���Ч
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // iv: integer, vector
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // alloca ��C���Ը����һ���������������ڶ�ջ�϶�̬�ط��䣬��������ж���ʹ��
        // ��Ϊ���ǲ����ڶ��Ϸ��䣬���Ե������������
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// ���ļ��л�ȡ��Ȼ����롢���ӣ�����buffer id������֮��İ�
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // �����ĵ�����֪������֤��״̬�ᱻ�洢Ϊ�������״̬��һ��������Ե��ã������� glGetProgram ��ѯʵ�ʽ����ʲô֮���
    glValidateProgram(program);

    // ��Ϊ�Ѿ������ӵ�һ���������ˣ����������Ը�����ɾ���м䲿��
    // ��C++�б��붫����ʱ���õ����� .obj �������м��ļ���shader Ҳ�����
    // ����һЩ�����ĺ��������� glDetachShader ֮�������ɾ��Դ����
    // ����cherno��ϲ������Щ������
    // ����������ЩҲ���Ǳ�Ҫ�ģ���Ϊ��ռ�õ��ڴ���٣����Ǳ�����ɫ����Դ������ڵ���֮����Ǻ���Ҫ��
    // ��˺ܶ������������ȥ���� glDetachShader ��Щ
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
	return location;
}
