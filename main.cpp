#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

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
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout
        << "Failed to compile "
        << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
        << " shader!"
        << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

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
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        cout << "ERRO!!" << endl;

    float positions[6] = {-1.0f, -1.0f,
                           0.0f,  1.0f,
                           1.0f, -1.0f};

    /* buffers. (num de buffers, endereço) -> como a função não tem retorno,
    ela escreve o valor do id na variável buffer. Por isso passamos o endereço dela */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // selecionando o buffer que acabei de criar
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // botar dados nesse buffer. size(float) para passar o número em bytes.
    // Static porque não vamos modificar o formato do triângulo. Senão, dinamic
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2* sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource,source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // sem index buffer
        // (triângulo, que começa da positions 0, e lê 3 duplas - vertices - de pontos)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
