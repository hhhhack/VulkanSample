#include "VulkanBase.h"

const static uint32_t WIDTH = 800;
const static uint32_t HEIGHT = 600;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

void HelloTriangleApplication::mainloop()
{
	while (!glfwWindowShouldClose(m_pWindows))
	{
		glfwPollEvents();
	}
}

void HelloTriangleApplication::run()
{
	initWindow();
	initVulkan();
	mainloop();
	cleanup();
}

void HelloTriangleApplication::initWindow()
{
	auto retCode = glfwInit();
	if (retCode != GLFW_TRUE)
	{
		throw std::exception("glfwInit fail");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	//m_pMonitor = glfwGetPrimaryMonitor();
	//if (!m_pMonitor)
	//{
	//	std::printf("glfwGetPrimaryMonitor fail");
	//}

	m_pWindows = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", m_pMonitor, nullptr);
	if (!m_pWindows)
	{
		throw std::exception("glfwCreateWindows fail");
	}

}

void HelloTriangleApplication::cleanup()
{
	vkDestroySwapchainKHR(m_Device, m_swapChain, nullptr);
	vkDestroyDevice(m_Device, nullptr);
	if (enableValidationLayers)
	{
		DestoryDebugUtilsMessengerEXT(m_vkInstance, debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(m_vkInstance, m_surface, nullptr);
	vkDestroyInstance(m_vkInstance, nullptr);
	if (m_pWindows)
		glfwDestroyWindow(m_pWindows);
	glfwTerminate();

}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
	uint32_t uLayerCount;
	vkEnumerateInstanceLayerProperties(&uLayerCount, nullptr);
	std::vector<VkLayerProperties>availableLayers(uLayerCount);
	vkEnumerateInstanceLayerProperties(&uLayerCount, availableLayers.data());

	for (auto& validationLayer : validationLayers)
	{
		bool bValidation = false;
		for (auto& availableLayer : availableLayers)
		{
			if (strcmp(availableLayer.layerName, validationLayer) == 0)
			{
				bValidation = true;
				break;
			}
		}
		if (!bValidation)
			return false;
	}
	return true;
}


std::vector<const char*> HelloTriangleApplication::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	uint32_t extensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
};


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageServerity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallBackData,
	void* pUserData)
{
	if (messageServerity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		std::cerr << "validation layer: " << messageType << " " << pCallBackData->pMessage << std::endl;
	return VK_FALSE;
};

void HelloTriangleApplication::createInstance()
{
	VkApplicationInfo appInfo{};
	VkInstanceCreateInfo createInfo{};
	std::vector<VkExtensionProperties> extensionProperties;
	VkDebugUtilsMessengerCreateInfoEXT debugCreateinfo{};

	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error("enable validation bug check fail");

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "MyEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;
	appInfo.pNext = nullptr;

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto reextensions = getRequiredExtensions();

	uint32_t uExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &uExtensionCount, nullptr);
	extensionProperties.resize(uExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &uExtensionCount, extensionProperties.data());
	for (auto supportExtension : reextensions)
	{
		bool bSupport = false;
		for (auto extensionProperty : extensionProperties)
		{
			if (strcmp(supportExtension, extensionProperty.extensionName) == 0)
			{
				bSupport = true;
				break;
			}
		}
		std::cout << "extension " << supportExtension << " is " << (bSupport ? "Support":"Not Support") << std::endl;
		if (!bSupport)
			throw std::runtime_error("extension is not support");
	}

	createInfo.enabledExtensionCount = reextensions.size();
	createInfo.ppEnabledExtensionNames = reextensions.data();

	if (!enableValidationLayers)
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}
	else
	{
		populateDebugMessengerCreateInfo(debugCreateinfo);
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.pNext = &debugCreateinfo;
	}
	if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
		throw std::runtime_error("vkCreateInstance fail");
}


VkResult HelloTriangleApplication::CreateDebugUtilsMessengerEXT(
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger
)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}


void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	createInfo.pfnUserCallback = debugCallBack;
	createInfo.pUserData = nullptr;
}


void HelloTriangleApplication::DestoryDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func)
		func(instance, debugMessenger, nullptr);
};

void HelloTriangleApplication::setupDebugMessenger()
{
	if (!enableValidationLayers)
		return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);
	if (CreateDebugUtilsMessengerEXT(m_vkInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		throw std::runtime_error("failed to setup debug messenger");
};

bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{
	bool bSwapChainAdequate = false;
	bool extensionsSupported = checkDeviceExtensionSupport(device);
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		bSwapChainAdequate = !swapChainSupport.formats.empty() &&
			!swapChainSupport.presentModes.empty();
	}

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader &&
		findQueueFamilies(device).has_value() &&
		extensionsSupported &&
		bSwapChainAdequate;
}

static int rateDeviceSuitability(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	int score = 0;
	if (!deviceFeatures.geometryShader)
		return 0;
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}
	score += deviceProperties.limits.maxImageDimension2D;
	return score;
}

void HelloTriangleApplication::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	std::vector<VkPhysicalDevice> devices;
	std::multimap<int, VkPhysicalDevice> candidates;
	vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUS");
	}
	//devices.reserve(deviceCount);
	devices.resize(deviceCount);

	vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());
	for (const auto& device : devices)
	{
		int score = rateDeviceSuitability(device);
		candidates.insert(std::pair<int, VkPhysicalDevice>(score, device));
	}
	if (candidates.size() && candidates.rbegin()->first > 0)
	{
		m_physicalDevice = candidates.rbegin()->second;
	}
	else
	{
		throw std::runtime_error("no suitable gups");
	}
	if (m_physicalDevice == nullptr)
		throw std::runtime_error("no suitable device");
};

QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice device)
{
	//std::optional<uint32_t> indices;
	QueueFamilyIndices indices;
	std::vector<VkQueueFamilyProperties> queueFamilyProperties;
	uint32_t uQueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &uQueueFamilyCount, nullptr);
	queueFamilyProperties.resize(uQueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &uQueueFamilyCount, queueFamilyProperties.data());
	int i = 0;
	for (auto queueproperty : queueFamilyProperties)
	{
		VkBool32 bPresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &bPresentSupport);
		if (bPresentSupport)
		{
			indices.presentFamily = i;
		}
		if (queueproperty.queueFlags | VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		i++;
	}
	return indices;
}

void HelloTriangleApplication::createLogicDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
	VkDeviceCreateInfo deviceCreateInfo;
	VkPhysicalDeviceFeatures deviceFeatures;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	VkQueue presentQueue;
	float queuePriority = 1.0f;

	deviceQueueCreateInfos.reserve(uniqueQueueFamilies.size());
	for (auto& queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo deviceQueueCreateInfo;
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
		deviceQueueCreateInfo.pNext = nullptr;
		deviceQueueCreateInfos.emplace_back(deviceQueueCreateInfo);
	}

	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = deviceQueueCreateInfos.size();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (enableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = validationLayers.size();
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}
	if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_Device) != VK_SUCCESS)
	{
		throw std::runtime_error("create device fail");
	}
	vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_presentQueue);
}

void HelloTriangleApplication::createSurface()
{
	if (glfwCreateWindowSurface(m_vkInstance, m_pWindows, nullptr, &m_surface) != VK_SUCCESS)
		throw std::runtime_error("create surface fail");
}

static bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount = 0;
	std::set<std::string> requiredExtensions;
	std::vector<VkExtensionProperties> availableExtensions;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	availableExtensions.resize(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	requiredExtensions = std::set<std::string>(deviceExtensions.begin(), deviceExtensions.end());

	for (auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	uint32_t uFormatCount = 0;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		device,
		m_surface,
		&details.capabilities
	);
	vkGetPhysicalDeviceSurfaceFormatsKHR(
		device,
		m_surface,
		&uFormatCount,
		nullptr
	);
	if (uFormatCount)
	{
		details.formats.resize(uFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
			device,
			m_surface,
			&uFormatCount,
			details.formats.data()
		);
	}

	return details;
}

static VkSurfaceFormatKHR chooseSwapChainSurfaceFromat(
	const std::vector<VkSurfaceFormatKHR>& availableFormats
)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
			)
			return availableFormat;
	}
	return availableFormats[0];
}

static VkPresentModeKHR chooseSwapPresentMode(
	const std::vector<VkPresentModeKHR>& availablePresentModes
)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAX_ENUM_KHR)
			return availablePresentMode;
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D HelloTriangleApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(m_pWindows, &width, &height);
		VkExtent2D actualExtent = { width, height };
		actualExtent.width = std::clamp(
			actualExtent.width,
			capabilities.minImageExtent.width,
			capabilities.maxImageExtent.width
		);
		actualExtent.height = std::clamp(
			actualExtent.height,
			capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height
		);
		return actualExtent;
	}
}

void HelloTriangleApplication::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapChainSurfaceFromat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
	VkSwapchainCreateInfoKHR swapChainCreateInfo;
	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
	uint32_t queueFamilyIndices[] = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};
	uint32_t uImageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 &&
		uImageCount > swapChainSupport.capabilities.maxImageCount
		)
		uImageCount = swapChainSupport.capabilities.maxImageCount;
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = m_surface;
	swapChainCreateInfo.minImageCount = uImageCount;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (indices.graphicsFamily != indices.presentFamily)
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}
	swapChainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = presentMode;
	swapChainCreateInfo.clipped = VK_TRUE;
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	if (vkCreateSwapchainKHR(m_Device, &swapChainCreateInfo, nullptr, &m_swapChain) !=
		VK_SUCCESS)
		throw std::runtime_error("failed to create swap chain");

};

void HelloTriangleApplication::createImageView() {

};

void HelloTriangleApplication::initVulkan()
{
	createInstance();
	//setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicDevice();
	createSwapChain();
	createImageView();
}

