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
        {
            fps = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    void Render(GLFWwindow *window)
    {
        Editor *handler = (Editor *)glfwGetWindowUserPointer(window);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);
        if (fps < 30)
        {
            color = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else if(fps > 60){
            color = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        handler->RenderText("FPS: ", Globals::VIEWPORT.RIGHT - 48, Globals::VIEWPORT.TOP, 1.0, glm::vec3(1.0f, 1.0f, 1.0f));
        handler->RenderText(std::to_string(fps), Globals::VIEWPORT.RIGHT - 18, Globals::VIEWPORT.TOP, 1.0, color);
    }

private:
    double lastTime;
    int frameCount;
    int fps;
};
