#ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "globals.h"
#include "core/editor.h"
#include "fpscounter.cpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(int argc, char *argv[])
{
    std::string fileName = "";
    if (argc > 1)
    {
        std::cout << "File provided: " << argv[1] << std::endl;
        fileName = argv[1];
    }
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(Globals::SCR_WIDTH, Globals::SCR_HEIGHT, "Minipad", NULL, NULL);

    if (window == NULL)
    {
        printf("Failed to create GLFW Window\n");
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OPENGL: context setter
    glfwMakeContextCurrent(window);

    // glad: initialize and configure
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load GLAD\n");
        return -1;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FPSCounter fpsCounter;
    Editor editor(Globals::SCR_WIDTH, Globals::SCR_HEIGHT, fileName);
    editor.SetCallbacks(window);

    // main: render loop
    while (!glfwWindowShouldClose(window))
    {
        fpsCounter.Update();
        glClearColor(0.1216f, 0.1216f, 0.1216f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        editor.Render();
        fpsCounter.Render(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}