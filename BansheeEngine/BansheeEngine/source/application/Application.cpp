#include "Application.h"
#include "foundation/Logger.h"
#include "foundation/INIParser.h"
#include "graphics/Window.h"
#include "graphics/Vulkan/VulkanRenderer.h"

namespace Banshee
{
	Application::Application()
	{
		BE_LOG(LogCategory::trace, "Banshee Engine initializing...");
		
		BE_LOG(LogCategory::trace, "Parsing configuration settings INI file");
		m_INIParser = std::make_unique<INIParser>();
		EngineConfig configSettings = m_INIParser->ParseConfigSettings("config.ini");

		m_Window = std::make_unique<Window>(configSettings.m_WindowWidth, configSettings.m_WindowHeight, configSettings.m_WindowTitle.c_str());

		BE_LOG(LogCategory::trace, "Vulkan initializing...");
		m_Renderer = std::make_unique<VulkanRenderer>(m_Window.get());
		BE_LOG(LogCategory::trace, "Vulkan initialized successfully");
	}

	Application::~Application()
	{
		BE_LOG(LogCategory::trace, "Banshee Engine shutting down");
	}

	void Application::Run() const
	{
		while (!m_Window->ShouldWindowClose())
		{
			m_Renderer->DrawFrame();
			m_Window->Update();
		}
	}
} // End of Banshee namespace