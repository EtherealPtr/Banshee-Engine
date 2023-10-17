#pragma once

typedef struct VkInstance_T* VkInstance;
typedef struct VkDebugUtilsMessengerEXT_T* VkDebugUtilsMessengerEXT;

namespace Banshee
{
	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

		VkInstance Get() const { return m_Instance; }

	private:
		void SetupDebugCallback();

	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};
}
