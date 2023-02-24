//
// main.cpp -точка входа
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>                 // библиотека GLAD для OpenGL 4.6
#define GLFW_INCLUDE_NONE              // предотвращает включение заголовка OpenGL в заголовок GLFW
#include <GLFW/include/GLFW/glfw3.h>   // библиотека GLFW

// точка входа
int main(int argc, char** args) {

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
    GLFWwindow *window = glfwCreateWindow(1024, 768, "Opty OpenGl Test", nullptr, nullptr);
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

    // основной цикл приложения
    while (!glfwWindowShouldClose(window)) // функция возвращает значение флага закрытия указанного окна
    {
        // меняет местами передний и задний буферы указанного окна
        glfwSwapBuffers(window);
        // обрабатывает только те события, которые уже находятся в очереди 
        // событий, а затем немедленно возвращается
        glfwPollEvents();
    }

    glfwTerminate(); // освобождает и уничтожает выделенные ресурсы GLFW

    return 0;
}
