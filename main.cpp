#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Windows
//#define ASSERT(x) if (!(x)) __debugbreak();
// MAC
#define ASSERT(x) if (!(x)) __builtin_trap();

#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

using namespace std;

static void GLClearError(){
    while(glGetError() != GL_NO_ERROR);

}

static bool GLLogCall(const char* function, const char* file, int line){
    while(GLenum error = glGetError()){
        cout << "[OpenGLError] (" << error << ") " << function << " " << file << ":" << line << endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource{
    string VertexSource;
    string FragmentSource;
};

static ShaderProgramSource ParseShader(const string& filepath){
    ifstream stream(filepath);

    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream,line)){
        if(line.find("#shader") != string::npos){
            if(line.find("vertex") != string::npos){
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != string::npos){
                type = ShaderType::FRAGMENT;
            }
        } else{
            ss[(int)type] << line << '\n';
        }
    }

    stream.close();

    return {ss[0].str(), ss[1].str()};
}

static int CompileShader(unsigned int type, const string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE){
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        cout
        << "Failed to compile "
        << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
        << " shader!"
        << endl;
        cout << message << endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader){
    GLCall(unsigned int program = glCreateProgram());
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        GLCall(glfwTerminate());
        return -1;
    }

    /* Make the window's context current */
    GLCall(glfwMakeContextCurrent(window));

    if (glewInit() != GLEW_OK)
        cout << "ERRO!!" << endl;

    float positions[] = {
                         -0.5f, -0.5f,
                          0.5f, -0.5f,
                          0.5f, 0.5f,
                          -0.5f, 0.5f};

    unsigned int indices[]{
        0, 1, 2,
        2, 3, 0
    };

    /* buffers. (num de buffers, endereço) -> como a função não tem retorno,
    ela escreve o valor do id na variável buffer. Por isso passamos o endereço dela */
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    // selecionando o buffer que acabei de criar
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    // botar dados nesse buffer. size(float) para passar o número em bytes.
    // Static porque não vamos modificar o formato do triângulo. Senão, dinamic
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource,source.FragmentSource);
    GLCall(glUseProgram(shader));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLClearError();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    GLCall(glDeleteProgram(shader));

    GLCall(glfwTerminate());
    return 0;
}
