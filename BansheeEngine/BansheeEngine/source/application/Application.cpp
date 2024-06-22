#include "Application.h"
#include "Foundation/Input/Input.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"

namespace Banshee
{
	Application::Application() :
		m_INIParser(std::make_unique<INIParser>()),
		m_Window(nullptr),
		m_Renderer(nullptr)
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee initializing");
		EngineConfig configSettings = m_INIParser->ParseConfigSettings("config.ini");
		m_Window = std::make_unique<Window>(configSettings.windowWidth, configSettings.windowHeight, configSettings.windowTitle.c_str());
		Input::Instance().Initialize(m_Window->GetWindow());
	}

	Application::~Application()
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee shutting down");
	}

	void Application::InitializeRenderer()
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Starting post client initialization");
		m_Renderer = std::make_unique<VulkanRenderer>(m_Window.get());
	}

	void Application::Run() const
	{
		BE_LOG(LogCategory::Trace, "[APPLICATION]: Banshee run");

		while (!m_Window->ShouldWindowClose())
		{
			m_Renderer->DrawFrame();
			Input::Instance().Update();
			m_Window->Update();
		}
	}
} // End of Banshee namespace