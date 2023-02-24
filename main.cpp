//
// main.cpp -точка входа
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>                 // библиотека GLAD для OpenGL 4.6
#define GLFW_INCLUDE_NONE              // предотвращает включение заголовка OpenGL в заголовок GLFW
#include <GLFW/include/GLFW/glfw3.h>   // библиотека GLFW

// массив вершин
float vertices[] = {
//    x      y      z
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
     0.0f,  0.5f,  0.0f
};

// вершинный шейдер 
static const char *vertexShaderSource = R"(
    #version 450 core
    layout (location = 0) in vec3 aPos;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

// фрагментный шейдер
static const char *fragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(0.7f, 0.5f, 0.2f, 1.0f);
    }
)";

// точка входа
int main(int argc, char **args)
{

    // иницилизация GLFW
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // устанавливает обратный вызов при ошибки
    glfwSetErrorCallback(
        [](int error, const char *description) // функция вызова
        {
            fprintf(stderr, "Error: %s\n", description);
        }
    );

    // настройка GLFW для работы с OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // старшая версия OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // младшая версия OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // выбор основного профиля OpenGL

    // создание окна
    GLFWwindow *window = glfwCreateWindow(800, 450, "OpenGl Test", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); // делает контекст OpenGL

    gladLoadGL(); // загрузка GLAD и иницилизация функций OpenGL

    /* устанавливает интервал подкачки для текущего контекста OpenGL, т.е.
       количество обновлений экрана, которые следует ожидать с момента 
       вызова glfwSwapBuffers перед заменой буферов и возвратом
    */
    glfwSwapInterval(1);

    // установка функции обратного вызова для кнопок
    glfwSetKeyCallback(
        window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods) // функция вызова
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    );

    // установка функции обратного вызова при изменении размера окна
    glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow *window, int width, int height)
        {
            // задает преобразование аффинных значений x и y из нормализованных 
            // координат устройства в координаты окна
            glViewport(0, 0, width, height);
        }
    );

    // строительство и компиляция шейдеров
    //------------------------------------------

    // вершинный шейдер
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // создание вершинного шейдера
    // устанавливает исходный код в шейдере
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
    // компиляция шейдера
    glCompileShader(vertexShader);
    // проверка на ошибки компиляции шейдера
    GLint success;
    char infoLog[1024]; // информационное сообщение
    // устанавливает значение параметра для конкретного объекта шейдера
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        // возвращает журнал сведений для указанного объекта шейдера
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
    }

    // фрагментный шейдер
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // создание фрагментного шейдера
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        // возвращает журнал сведений для указанного объекта шейдера
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
    }

    // создание шейдерной программы и линковка к ней шейдеров
    GLuint shaderProgram = glCreateProgram(); // создание шейдерной программы
    glAttachShader(shaderProgram, vertexShader); // присоединение вершинного шейдера
    glAttachShader(shaderProgram, fragmentShader); // присоединение фрагментного шейдера
    glLinkProgram(shaderProgram); // связывает объекты шейдерной программы
    // проверка линковки
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR: Shader Program linking failed: %s", infoLog);
    }

    // после линковки в программу, шейдеры можно удалить
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // установка данных вершин (и буферов) и конфигурация атрибутов вершин
    //----------------------------------------------------------------------

    // создание объекта массива вершин (VAO)
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // создание объекта буфера вершин (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);

    /* сначала привяжите объект массива вершин, затем привяжите и установите 
       буфер(ы) вершин, а затем настройте атрибуты вершин.
    */
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    /* обратите внимание, что это разрешено, вызов glVertexAttribPointer
       регистрирует VBO как привязанный объект вершинного буфера атрибута вершины,
       поэтому впоследствии мы можем безопасно отменить привязку
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* После этого вы можете отменить привязку VAO, чтобы другие вызовы VAO
       случайно не изменили этот VAO, но это случается редко. Изменение другого
       VAO в любом случае требует вызова glBindVertexArray, поэтому обычно мы не
       отвязываем VAO (или VBO), когда в этом нет прямой необходимости.
    */
    glBindVertexArray(0);


    // основной цикл приложения
    while (!glfwWindowShouldClose(window)) // возвращает значение флага закрытия указанного окна
    {
        // очистка
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); // цвет очистки
        glClear(GL_COLOR_BUFFER_BIT); // очистка цветового буфера

        // рендеринг
        //-------------------------

        // устанавливает программный объект как часть текущего состояния рендеринга
        glUseProgram(shaderProgram);
        /* поскольку у нас есть только один VAO, нет необходимости привязывать 
           его каждый раз, но мы сделаем это, чтобы все было более организованно
        */
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // меняет местами передний и задний буферы указанного окна
        glfwSwapBuffers(window);
        // обрабатывает только те события, которые уже находятся в очереди 
        // событий, а затем немедленно возвращается
        glfwPollEvents();
    }

    // отменить выделение всех ресурсов, когда они изжили свое предназначение
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shaderProgram);

    glfwTerminate(); // освобождает и уничтожает выделенные ресурсы GLFW

    return 0;
}
