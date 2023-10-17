#include "Application.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"

namespace Banshee
{
	Application::Application()
	{
		m_INIParser = std::make_unique<INIParser>();
		EngineConfig configSettings = m_INIParser->ParseConfigSettings("config.ini");

		m_Window = std::make_unique<Window>(configSettings.m_WindowWidth, configSettings.m_WindowHeight, configSettings.m_WindowTitle.c_str());
		m_Renderer = std::make_unique<VulkanRenderer>(m_Window.get());

		BE_LOG(LogCategory::Trace, "Banshee initialized");
	}

	Application::~Application()
	{}

	void Application::Run() const
	{
		while (!m_Window->ShouldWindowClose())
		{
			m_Renderer->DrawFrame();
			m_Window->Update();
		}
	}
} // End of Banshee namespace