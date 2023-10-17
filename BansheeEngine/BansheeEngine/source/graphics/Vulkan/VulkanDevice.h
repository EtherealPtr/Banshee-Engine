#pragma once

#include <cstdint>

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
		bool Validate() const
		{
			return (m_GraphicsQueueFamilyIndex != UINT32_MAX) &&
				(m_TransferQueueFamilyIndex != UINT32_MAX) &&
				(m_PresentationQueueFamilyIndex != UINT32_MAX);
		}

		uint32_t m_GraphicsQueueFamilyIndex{ UINT32_MAX };
		uint32_t m_TransferQueueFamilyIndex{ UINT32_MAX };
		uint32_t m_PresentationQueueFamilyIndex{ UINT32_MAX };
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(const VkInstance& _vkInstance, const VkSurfaceKHR& _vkSurface);
		~VulkanDevice();

		VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetLogicalDevice() const { return m_LogicalDevice; }
		DeviceQueueIndices GetQueueIndices() const { return m_QueueIndices; }
		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetPresentationQueue() const { return m_PresentQueue; }
		VkQueue GetTransferQueue() const { return m_TransferQueue; }
		VkPhysicalDeviceLimits GetLimits();

	private:
		void PickPhysicalDevice(const VkInstance& _vkInstance);
		uint32_t RateDeviceSuitability(const VkPhysicalDevice& _gpu, const VkPhysicalDeviceProperties& _deviceProperties);
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
}