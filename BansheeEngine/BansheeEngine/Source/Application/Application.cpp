#include "Application.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Foundation/Timer/Timer.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"

namespace Banshee
{
	Application::Application() :
		m_Window{ nullptr },
		m_Renderer{ nullptr }
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee initializing");
		const EngineConfig configSettings{ INIParser::ParseConfigSettings("config.ini") };
		m_Window = std::make_unique<Window>(configSettings.m_WindowWidth, configSettings.m_WindowHeight, configSettings.m_WindowTitle);
	}

	Application::~Application()
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee terminating");
	}

	void Application::PostClientInitialize()
	{
		try 
		{ 
			m_Renderer = std::make_unique<VulkanRenderer>(*m_Window.get()); 
			BE_LOG(LogCategory::Info, "[APPLICATION]: VulkanRenderer initialized successfully"); 
		}
		catch (const std::exception& _problem)
		{
			BE_LOG(LogCategory::Error, "[APPLICATION]: Failed to initialize VulkanRenderer: %s", _problem.what());
		}
	}

	void Application::Run() const
	{
		Timer timer{};
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee run");

		while (!m_Window->ShouldWindowClose())
		{
			timer.Update();
			m_Window->PollEvents();
			m_Renderer->DrawFrame(timer.GetDeltaTime());
		}
	}
} // End of Banshee namespace