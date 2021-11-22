#ifndef OPENGL_VERTEXBUFFER_H
#define OPENGL_VERTEXBUFFER_H
#endif //OPENGL_VERTEXBUFFER_H

class VertexBuffer{
    private:
    // ID para determinar um objeto, ou buffer
        unsigned int m_RendererID;
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
};