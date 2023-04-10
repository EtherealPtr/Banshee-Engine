#include "VulkanInstance.h"
#include "foundation/Logger.h"
#include "foundation/Platform.h"
#include "VulkanUtils.h"
#include "VulkanValidation.h"
#include <vulkan/vulkan.h>
#include <glfw3.h>
#include <vector>
#include <stdexcept>

#ifdef NDEBUG
	const bool g_EnableValidationLayers = false;
#else
	const bool g_EnableValidationLayers = true;
#endif

namespace Banshee
{
	VulkanInstance::VulkanInstance() : 
		m_Instance(VK_NULL_HANDLE),
		m_DebugMessenger(VK_NULL_HANDLE)
	{
		BE_LOG(LogCategory::trace, "Vulkan instance creation stage: Start");

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Untitled";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
		appInfo.pEngineName = "Banshee";
		appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;

		// Query the required extensions for GLFW
		uint32 glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		
		// List the required instance extensions
		std::vector<const char*> requiredInstanceExtensions;
		requiredInstanceExtensions.reserve((size_t)glfwExtensionCount + 1);

		for (uint8 i = 0; i < glfwExtensionCount; ++i)
		{
			requiredInstanceExtensions.emplace_back(glfwExtensions[i]);
		}

		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;

		std::vector<const char*> validationLayers;

		if (g_EnableValidationLayers)
		{
			requiredInstanceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);;

			// Enable validation layers
			validationLayers.reserve(1);
			validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

			VulkanUtils::CheckInstanceLayerSupport(validationLayers);
			instanceCreateInfo.enabledLayerCount = static_cast<uint32>(validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}

		// Enable instance extensions
		VulkanUtils::CheckInstanceExtSupport(requiredInstanceExtensions);
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32>(requiredInstanceExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions.data();

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::error, "Failed to create a Vulkan instance");
			throw std::runtime_error("ERROR: Failed to create Vulkan instance\n");
		}

		BE_LOG(LogCategory::info, "Vulkan instance created successfully");

		SetupDebugCallback();

		BE_LOG(LogCategory::trace, "Vulkan instance creation stage: End");
	}

	VulkanInstance::~VulkanInstance()
	{
		if (g_EnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
			m_DebugMessenger = VK_NULL_HANDLE;
			BE_LOG(LogCategory::trace, "Vulkan debug messenger destroyed");
		}

		vkDestroyInstance(m_Instance, nullptr);
		m_Instance = VK_NULL_HANDLE;
		BE_LOG(LogCategory::trace, "Vulkan instance destroyed");
	}

	void VulkanInstance::SetupDebugCallback()
	{
		if (!g_EnableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = nullptr;

		if (CreateDebugUtilsMessengerEXT(m_Instance, &debugCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::warning, "Failed to create vulkan debug messenger");
		}
		else
		{
			BE_LOG(LogCategory::info, "Vulkan debug messenger created successfully");
		}
	}

} // End of Banshee namespace