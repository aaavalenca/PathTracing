#ifndef OPENGL_VERTEXBUFFER_H
#define OPENGL_VERTEXBUFFER_H
#endif //OPENGL_VERTEXBUFFER_H

class IndexBuffer{
    private:
    // ID para determinar um objeto, ou buffer
        unsigned int m_RendererID;
        unsigned int m_Count;
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const { return m_Count; }
};