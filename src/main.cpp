#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec4 color;"
"void main()\n"
"{\n"
"    vec4 pos = { aPos.x, aPos.y, aPos.z, 1.0f };\n"
"    gl_Position = pos;\n"
"    color = vec4(aColor.xyz, 1.0f);\n"
"}";

const char* fragmentShaderSource =
"#version 330 core\n"
"in vec4 color;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = color;\n"
"}";

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
    {
        int successCompile;

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successCompile);
        if (!successCompile)
            std::cout << "Failed compile vertex shader." << std::endl;

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successCompile);
        if (!successCompile)
            std::cout << "Failed compile fragment shader." << std::endl;

        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShader);
        glAttachShader(m_id, fragmentShader);

        glLinkProgram(m_id);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() const
    {
        glUseProgram(m_id);
    }
private:
    unsigned int m_id;
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() 
{
    glfwInit(); // Инициализация библиотеки GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Сделать контекст окна главным в текущем потоке

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    // ----
    ShaderProgram shaderProgram(vertexShaderSource, fragmentShaderSource);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO ---
    /*
        VBO — способ хранения некоторых данных на видеокарте
    */
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // Назначаем VBO в качестве GL_ARRAY_BUFFER
    // GL_ARRAY_BUFFER — цель
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Скопировать некоторые данные, определенные пользователем, в буфер
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /*
    * GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    * GL_STATIC_DRAW: the data is set only once and used many times.
    * GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */
    // -------

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    int indices[] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // ----

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
