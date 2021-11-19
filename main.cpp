#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>
using namespace std;

static int CreateShader(const string& vertexShader, const string& fragmentShader){

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

    glfwTerminate();
    return 0;
}
