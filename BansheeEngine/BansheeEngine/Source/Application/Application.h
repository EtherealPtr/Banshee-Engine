#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/INIParser.h"
#include <memory>

namespace Banshee
{
	class Window;
	class VulkanRenderer;

	class Application
	{
	public:
		BANSHEE_ENGINE Application();
		BANSHEE_ENGINE virtual ~Application();

		BANSHEE_ENGINE void InitializeRenderer();
		BANSHEE_ENGINE void Run() const;

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		INIParser m_INIParser;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<VulkanRenderer> m_Renderer;
	};
} // End of Banshee namespace
