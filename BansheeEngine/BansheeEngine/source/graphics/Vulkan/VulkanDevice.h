#pragma once

#include "Foundation/Platform.h"

typedef struct VkInstance_T* VkInstance;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
typedef struct VkQueue_T* VkQueue;
struct VkPhysicalDeviceProperties;
struct VkPhysicalDeviceLimits;

namespace Banshee
{
	struct DeviceQueueIndices
	{
		bool Validate() const noexcept
		{
			return (graphicsQueueFamilyIndex != UINT32_MAX) &&
				   (transferQueueFamilyIndex != UINT32_MAX) &&
				   (presentationQueueFamilyIndex != UINT32_MAX);
		}

		uint32 graphicsQueueFamilyIndex{ UINT32_MAX };
		uint32 transferQueueFamilyIndex{ UINT32_MAX };
		uint32 presentationQueueFamilyIndex{ UINT32_MAX };
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(const VkInstance& _vkInstance, const VkSurfaceKHR& _vkSurface);
		~VulkanDevice();

		VkPhysicalDevice GetPhysicalDevice() const noexcept { return m_PhysicalDevice; }
		VkDevice GetLogicalDevice() const noexcept { return m_LogicalDevice; }
		DeviceQueueIndices GetQueueIndices() const noexcept { return m_QueueIndices; }
		VkQueue GetGraphicsQueue() const noexcept { return m_GraphicsQueue; }
		VkQueue GetPresentationQueue() const noexcept { return m_PresentQueue; }
		VkQueue GetTransferQueue() const noexcept { return m_TransferQueue; }
		VkPhysicalDeviceLimits GetLimits();

	private:
		void PickPhysicalDevice(const VkInstance& _vkInstance);
		bool CheckDeviceFeatures(const VkPhysicalDevice& _gpu);
		uint32 RateDeviceSuitability(const VkPhysicalDevice& _gpu, const VkPhysicalDeviceProperties& _deviceProperties);
		void SetupQueueFamilyIndices();
		void CreateLogicalDevice();

	private:
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;
		DeviceQueueIndices m_QueueIndices;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_TransferQueue;
		VkQueue m_PresentQueue;
	};
} // End of Banshee namespace