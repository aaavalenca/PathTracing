#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H
#endif //OPENGL_RENDERER_H
#include <GL/glew.h>
// Windows
//#define ASSERT(x) if (!(x)) __debugbreak();
// MAC
#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

using namespace std;

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
