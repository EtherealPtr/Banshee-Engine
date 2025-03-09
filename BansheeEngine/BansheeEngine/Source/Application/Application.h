#pragma once

#include "Foundation/DLLConfig.h"
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

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		BANSHEE_ENGINE void PostClientInitialize();
		BANSHEE_ENGINE void Run() const;

	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<VulkanRenderer> m_Renderer;
	};
} // End of namespace
