#pragma once

#include <vector>
#include "Renderer.h"
/*
glVertexAttribPointer

index���ڼ������ԣ���0��ʼȡ��0��1��2��˳���Լ����壬���綥��λ�ã���������

����ֻ�ж���λ�ã�Ҳֻ�����۶���λ�ã�����Ϊ0

size��һ�������������ݵĸ���������ÿ����������������������ֵ��������2

type�������������ݵ����ͣ�����position�����е�����ȫ��Ϊfloat��������GL_FLOAT

normalized���Ƿ���Ҫ�Կ���æ�����ݹ�һ����-1��+1���䣬���ﲻ��Ҫ����������GL_FALSE

stride��һ������ռ�е��ܵ��ֽ���������Ϊ����float��������sizeof(float)*2

pointer����ǰָ��ָ���vertex�ڲ���ƫ���ֽ���������Ψһ�ı�ʶ����ĳ�����Ե�ƫ����

������ָ���һ�����ԣ��������꣬ƫ����Ϊ0

*/
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized; // �� unsigned char ���� bool ��Ҫ����ֱ�Ӹ� glVertexAttribPointer �������ĸ�����

	static unsigned int GetSizeOfType(unsigned int type) // ��Ϊ����� type ��ʵ�� type ��һ����־��Ҳ���� GL_FLOAT ��ʾʵ�ʵ��� float��GLfloat��������ʵ���� type �� unsigned int ���͵�
	{
		switch (type)
		{
			case GL_FLOAT:			 return 4;
			case GL_UNSIGNED_INT:    return 4;
			case GL_UNSIGNED_BYTE:   return 1;
			default:
				break;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}
	
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};