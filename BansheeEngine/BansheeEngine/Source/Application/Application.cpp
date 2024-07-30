#include "Application.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Foundation/Timer/Timer.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"

namespace Banshee
{
	Application::Application() :
		m_INIParser{ std::make_unique<INIParser>() },
		m_Window{ nullptr },
		m_Renderer{ nullptr },
		m_Timer{ std::make_unique<Timer>() }
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee initializing");
		const EngineConfig configSettings = m_INIParser->ParseConfigSettings("config.ini");
		m_Window = std::make_unique<Window>(configSettings.m_WindowWidth, configSettings.m_WindowHeight, configSettings.m_WindowTitle.c_str());
	}

	Application::~Application()
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee shutting down");
	}

	void Application::InitializeRenderer()
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Beginning post-client initialization step");
		m_Renderer = std::make_unique<VulkanRenderer>(m_Window.get());
	}

	void Application::Run() const
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee run");

		while (!m_Window->ShouldWindowClose())
		{
			m_Timer->Update();
			m_Renderer->DrawFrame(m_Timer->GetDeltaTime());
			m_Window->PollEvents();
		}
	}
} // End of Banshee namespace