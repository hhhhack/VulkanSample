#pragma once

#if defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

#include <stdexcept>
#include <print>
#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <algorithm>
#include <limits>

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool has_value()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class HelloTriangleApplication
{
public:
	HelloTriangleApplication() :
		m_pWindows(nullptr), 
		m_pMonitor(nullptr), 
		m_physicalDevice(VK_NULL_HANDLE), 
		m_Device(VK_NULL_HANDLE)
	{};

	~HelloTriangleApplication() {};
	void run();

private:
	GLFWwindow* m_pWindows;
	GLFWmonitor* m_pMonitor;
	VkInstance m_vkInstance;

	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_Device;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapChain;
	std::vector<VkImage> m_swapChainImages;

private:
	void initWindow();

	bool checkValidationLayerSupport();

	std::vector<const char*> getRequiredExtensions();

	void createInstance();

	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger
	);

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void DestoryDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	void setupDebugMessenger();

	bool isDeviceSuitable(VkPhysicalDevice device);

	void pickPhysicalDevice();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void createLogicDevice();

	void createSurface();

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createSwapChain();

	void createImageView();

	void initVulkan();

	void mainloop();

	void cleanup();
};


static bool checkDeviceExtensionSupport(VkPhysicalDevice device);