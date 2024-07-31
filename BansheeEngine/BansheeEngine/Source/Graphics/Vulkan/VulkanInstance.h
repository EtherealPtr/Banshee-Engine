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

		VkInstance Get() const noexcept { return m_Instance; }

		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance& operator=(const VulkanInstance&) = delete;
		VulkanInstance(VulkanInstance&&) = delete;
		VulkanInstance& operator=(VulkanInstance&&) = delete;

	private:
		void SetupDebugCallback();

	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};
} // End of Banshee namespace