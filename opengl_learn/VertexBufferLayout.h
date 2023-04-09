#pragma once

#include <vector>
#include "Renderer.h"
/*
glVertexAttribPointer

index：第几个属性，从0开始取，0，1，2，顺序自己定义，例如顶点位置，纹理，法线

这里只有顶点位置，也只能讨论顶点位置，所以为0

size：一个顶点所有数据的个数，这里每个顶点又两个浮点数属性值，所以是2

type：顶点描述数据的类型，这里position数组中的数据全部为float，所以是GL_FLOAT

normalized：是否需要显卡帮忙把数据归一化到-1到+1区间，这里不需要，所以设置GL_FALSE

stride：一个顶点占有的总的字节数，这里为两个float，所以是sizeof(float)*2

pointer：当前指针指向的vertex内部的偏离字节数，可以唯一的标识顶点某个属性的偏移量

这里是指向第一个属性，顶点坐标，偏移量为0

*/
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized; // 用 unsigned char 而非 bool 主要是想直接给 glVertexAttribPointer 当作第四个参数

	static unsigned int GetSizeOfType(unsigned int type) // 因为这里的 type 是实际 type 的一个标志，也就是 GL_FLOAT 表示实际的是 float（GLfloat），但是实际上 type 是 unsigned int 类型的
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