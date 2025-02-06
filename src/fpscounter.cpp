#include <string>
#include "globals.h"
#include "./renderer/text/text.h"
#include "./core/editor.h"

class FPSCounter
{
public:
    FPSCounter() : lastTime(glfwGetTime()), frameCount(0), fps(0) {}

    void Update()
    {
        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - lastTime >= 1.0)
        { // Every second
            fps = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    void Render(GLFWwindow *window)
    {
        Editor *handler = (Editor *)glfwGetWindowUserPointer(window);
        std::string fpsText = "FPS: " + std::to_string(fps);
        handler->RenderText(fpsText, Globals::VIEWPORT.RIGHT - 50, Globals::VIEWPORT.TOP, 1.0, glm::vec3(1.0f, 1.0f, 0.0f));
    }

private:
    double lastTime;
    int frameCount;
    int fps;
};
