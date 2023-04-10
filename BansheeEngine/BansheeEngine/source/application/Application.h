#pragma once

#include "foundation/DLLConfig.h"
#include <memory>

namespace Banshee
{
	class Window;
	class VulkanRenderer;
	class INIParser;

	class Application
	{
	public:
		BANSHEE_ENGINE Application();
		BANSHEE_ENGINE ~Application();
		
		BANSHEE_ENGINE void Run() const;

	private:
		std::unique_ptr<INIParser> m_INIParser;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<VulkanRenderer> m_Renderer;
	};
} // End of Banshee namespace
