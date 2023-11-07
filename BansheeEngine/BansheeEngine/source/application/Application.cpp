#include "Application.h"
#include "Input/Input.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"

namespace Banshee
{
	Application::Application()
	{
		BE_LOG(LogCategory::Trace, "Banshee initializing");
		BE_LOG(LogCategory::Trace, "Loading config.ini");

		m_INIParser = std::make_unique<INIParser>();
		EngineConfig configSettings = m_INIParser->ParseConfigSettings("config.ini");
		BE_LOG(LogCategory::Info, "Loaded config.ini");

		BE_LOG(LogCategory::Trace, "Creating window");
		m_Window = std::make_unique<Window>(configSettings.windowWidth, configSettings.windowHeight, configSettings.windowTitle.c_str());
		BE_LOG(LogCategory::Info, "Created window");

		BE_LOG(LogCategory::Trace, "Initializing Vulkan");
		m_Renderer = std::make_unique<VulkanRenderer>(m_Window.get());
		BE_LOG(LogCategory::Info, "Vulkan initialized");

		BE_LOG(LogCategory::Trace, "Initializing input system");
		Input::Instance().Initialize(m_Window->GetWindow());
		BE_LOG(LogCategory::Info, "Input system initialized");
		BE_LOG(LogCategory::Info, "Banshee initialized successfully");
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