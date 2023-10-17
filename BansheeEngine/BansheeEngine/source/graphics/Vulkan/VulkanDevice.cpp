#include "VulkanDevice.h"
#include "VulkanUtils.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <stdexcept>
#include <cassert>

namespace Banshee
{
	VulkanDevice::VulkanDevice(const VkInstance& _vkInstance, const VkSurfaceKHR& _vkSurface) :
		m_PhysicalDevice(nullptr),
		m_LogicalDevice(nullptr),
		m_QueueIndices{},
		m_Surface(_vkSurface),
		m_GraphicsQueue(VK_NULL_HANDLE),
		m_TransferQueue(VK_NULL_HANDLE),
		m_PresentQueue(VK_NULL_HANDLE)
	{
		PickPhysicalDevice(_vkInstance);
		SetupQueueFamilyIndices();

		CreateLogicalDevice();
	}

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(m_LogicalDevice, nullptr);
		m_LogicalDevice = VK_NULL_HANDLE;
	}

	void VulkanDevice::PickPhysicalDevice(const VkInstance& _vkInstance)
	{
		unsigned int deviceCount = 0;
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			throw std::runtime_error("ERROR: Failed to find GPUs\n");
		}

		std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(_vkInstance, &deviceCount, physicalDevices.data());

		std::string preferredDeviceName = "";
		uint32_t maxScore = 0;

		for (const auto& gpu : physicalDevices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(gpu, &deviceProperties);
			vkGetPhysicalDeviceFeatures(gpu, &deviceFeatures);

			uint32_t deviceScore = RateDeviceSuitability(gpu, deviceProperties);

			if (deviceScore > maxScore)
			{
				maxScore = deviceScore;
				m_PhysicalDevice = gpu;
				preferredDeviceName = deviceProperties.deviceName;
			}
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("ERROR: Failed to pick a suitable GPU\n");
		}
	}

	unsigned int VulkanDevice::RateDeviceSuitability(const VkPhysicalDevice& _gpu, const VkPhysicalDeviceProperties& _deviceProperties)
	{
		unsigned int queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &queueFamilyCount, queueFamilies.data());

		VkBool32 graphicsSupport = VK_FALSE;
		VkBool32 presentSupport = VK_FALSE;

		for (uint32_t i = 0; i < queueFamilies.size(); ++i)
		{
			if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsSupport = VK_TRUE;
			}

			vkGetPhysicalDeviceSurfaceSupportKHR(_gpu, i, m_Surface, &presentSupport);

			if (graphicsSupport && presentSupport)
			{
				break;
			}
		}

		if (!graphicsSupport || !presentSupport) return 0;

		uint32_t deviceScore = 0;

		if (_deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			deviceScore += 1000;
		}

		deviceScore += _deviceProperties.limits.maxImageDimension2D;

		return deviceScore;
	}

	void VulkanDevice::SetupQueueFamilyIndices()
	{
		assert(m_PhysicalDevice != VK_NULL_HANDLE);

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

		for (uint32_t i = 0; i < queueFamilyCount; ++i)
		{
			if (m_QueueIndices.m_GraphicsQueueFamilyIndex == UINT32_MAX)
			{
				if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					m_QueueIndices.m_GraphicsQueueFamilyIndex = i;
				}
			}

			if (m_QueueIndices.m_TransferQueueFamilyIndex == UINT32_MAX)
			{
				if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
				{
					m_QueueIndices.m_TransferQueueFamilyIndex = i;
				}
			}

			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_Surface, &presentSupport);

			if (m_QueueIndices.m_PresentationQueueFamilyIndex == UINT32_MAX && presentSupport)
			{
				m_QueueIndices.m_PresentationQueueFamilyIndex = i;
			}
		}

		if (!m_QueueIndices.Validate())
		{
			throw std::runtime_error("ERROR: Queue indices are invalid, check GPU specifications");
		}
	}

	void VulkanDevice::CreateLogicalDevice()
	{
		std::set<uint32_t> queueIndices = { m_QueueIndices.m_GraphicsQueueFamilyIndex,
										  m_QueueIndices.m_TransferQueueFamilyIndex,
										  m_QueueIndices.m_PresentationQueueFamilyIndex };

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(queueIndices.size());

		float queuePriority{ 1.0f };

		uint32_t index = 0;
		for (const auto& queueIndex : queueIndices)
		{
			// Specify device queue create info
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos[index] = queueCreateInfo;
		}

		// Specify device features
		VkPhysicalDeviceFeatures deviceFeatures{};
		//deviceFeatures.fillModeNonSolid = VK_TRUE;

		std::vector<const char*> deviceExtentions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VulkanUtils::CheckDeviceExtSupport(m_PhysicalDevice, deviceExtentions);

		// Specify device create info
		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtentions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtentions.data();

		if (vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create logical device\n");
		}

		vkGetDeviceQueue(m_LogicalDevice, m_QueueIndices.m_GraphicsQueueFamilyIndex, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_QueueIndices.m_TransferQueueFamilyIndex, 0, &m_TransferQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_QueueIndices.m_PresentationQueueFamilyIndex, 0, &m_PresentQueue);
	}

	VkPhysicalDeviceLimits VulkanDevice::GetLimits()
	{
		VkPhysicalDeviceProperties gpuProperties{};
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &gpuProperties);
		return gpuProperties.limits;
	}
}
