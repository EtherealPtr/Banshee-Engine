#include "Application.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"
#include "Graphics/Input/Input.h"

namespace Banshee
{
	Application::Application()
	{
		BE_LOG(LogCategory::Trace, "Banshee initializing");

		m_INIParser = std::make_unique<INIParser>();
		EngineConfig configSettings = m_INIParser->ParseConfigSettings("config.ini");

		m_Window = std::make_unique<Window>(configSettings.windowWidth, configSettings.windowHeight, configSettings.windowTitle.c_str());
		m_Renderer = std::make_unique<VulkanRenderer>(m_Window.get());

		Input::Instance().Initialize(m_Window->GetWindow());

		BE_LOG(LogCategory::Trace, "Banshee initialized");
	}

	Application::~Application()
	{}

	void Application::Run() const
	{
		while (!m_Window->ShouldWindowClose())
		{
			m_Renderer->DrawFrame();
			Input::Instance().Update();
			m_Window->Update();
		}
	}
} // End of Banshee namespace