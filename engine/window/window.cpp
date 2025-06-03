#include "window.h"

Window::Window(int width, int height, const std::string& title) : 
    m_width(width), m_height(height), m_title(title), m_window(nullptr), m_framebufferResized(false)
{
    initWindow();
}

void Window::initWindow()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, frameBufferResizeCallBack);
}
Window::~Window()
{
    if(m_window)
    {
        glfwDestroyWindow(m_window);
    }

    glfwTerminate();
}


bool Window::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

GLFWwindow *Window::getWindow() const
{
    return m_window;
}

void Window::frameBufferResizeCallBack(GLFWwindow *window, int width, int height)
{
    auto windowInstance = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    if(windowInstance)
    {
        windowInstance->m_framebufferResized = true;
    }
}
