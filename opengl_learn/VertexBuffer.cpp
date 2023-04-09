#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    /*
    * glGenBuffers
    n
    Specifies the number of buffer object names to be generated.

    buffers
    Specifies an array in which the generated buffer object names are stored.
    */
    GLCall(glGenBuffers(1, &m_RendererID));
    /*
    * 
    target
Specifies the target to which the buffer object is bound
    buffer
Specifies the name of a buffer object.
    */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//select buffer
    /*
    target
Specifies the target to which the buffer object is bound for glBufferData
    buffer
Specifies the name of the buffer object for glNamedBufferData function.

size
Specifies the size in bytes of the buffer object's new data store.

data
Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.

usage
Specifies the expected usage pattern of the data store. The symbolic constant must be GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.
    */

    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
