#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void initWindow();
    bool shouldClose();
    void pollEvents();
    GLFWwindow* getWindow() const;

    bool wasResized()
    {
        bool resized = m_framebufferResized;
        m_framebufferResized = false;
        return resized;
    }

    VkExtent2D getExtent() const
    {
        if(!m_window) return { 0, 0 };
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    }
    
private:
    static void frameBufferResizeCallBack(GLFWwindow* window, int width, int height);
    
private:
        int m_width;
        int m_height;
        GLFWwindow* m_window;
        std::string m_title;
        bool m_framebufferResized;
};
#endif // WINDOW_H