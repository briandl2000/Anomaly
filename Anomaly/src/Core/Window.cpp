#include "anompch.h"

#include "Window.h"

#include "Application.h"
#include "GLFW/glfw3.h"


namespace Anomaly
{
	static b8 s_GLFWInitialized = FALSE;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ANOM_CORE_ERROR("GLFW Error (%l): %s", error, description);
	}

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		ShutDown();
	}

	u32 Window::GetWidth()
	{
		return m_Data.Width;
	}

	u32 Window::GetHeight()
	{
		return m_Data.Height;
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
	}

	void Window::Init(const WindowProps& props)
	{
		ANOM_CORE_INFO("Creating window: %s, (%lu, %lu)", props.Title, props.Width, props.Height);


		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (!s_GLFWInitialized)
		{
			u32 success = glfwInit();
			ANOM_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = TRUE;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title, nullptr, nullptr);

		glfwSetWindowUserPointer(m_Window, &m_Data);

		ANOM_CORE_INFO("Window Created.");

	}

	void Window::ShutDown()
	{
		ANOM_CORE_INFO("Shutting down Window...");
	}
}
