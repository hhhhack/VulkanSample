#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
#include <map>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

bool checkValidationLayerSupport()
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
			if (strcmp(availableLayer.layerName, validationLayer)==0)
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

class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainloop();
		cleanup();
	}
	HelloTriangleApplication() :m_pWindows(nullptr){}
private:
	GLFWwindow* m_pWindows;
	VkInstance m_vkInstance;

	void initWindow()
	{
		auto retCode = glfwInit();
		if (retCode != GLFW_TRUE)
		{
			throw std::exception("glfwInit fail");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_pWindows = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		if (!m_pWindows)
		{
			throw std::exception("glfwCreateWindows fail");
		}
	}

	void createInstance()
	{
		VkApplicationInfo appInfo{};
		VkInstanceCreateInfo createInfo{};
		uint32_t glfwExtensionCount = 0;
		uint32_t extensionCount = 0;
		const char** glfwExtensions = nullptr;
		VkResult result;

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		std::vector<VkExtensionProperties> extensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		//result = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);
		if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
			throw std::runtime_error("vkCreateInstance fail");

		result = vkEnumerateInstanceExtensionProperties(
			nullptr, 
			&extensionCount, 
			nullptr
		);
		if (result != VK_SUCCESS)
			throw std::runtime_error("error");
		extensions = std::vector<VkExtensionProperties>(extensionCount);

		result = vkEnumerateInstanceExtensionProperties(
			nullptr,
			&extensionCount,
			extensions.data()
		);

		for (auto& extension : extensions)
		{
			std::println("extension name {}", extension.extensionName);
		}
		if (enableValidationLayers && !checkValidationLayerSupport())
			throw std::runtime_error("validation not available");
		if (!enableValidationLayers)
		{
			createInfo.enabledLayerCount = 0;
		}
		else
		{
			createInfo.enabledLayerCount = 1;
			createInfo.ppEnabledExtensionNames = validationLayers.data();
		}
	}

	void setupDebugMessenger()
	{

	}

	bool isDeviceSuitable(VkPhysicalDevice vkdevice)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(vkdevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(vkdevice, &deviceFeatures);
		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			deviceFeatures.geometryShader;
	}

	void pickPhysicalDevice()
	{
		VkPhysicalDevice pVkPhydevice = VK_NULL_HANDLE;
		uint32_t udeviceCount = 0;
		std::vector<VkPhysicalDevice> devices;
		std::multimap<int, VkPhysicalDevice> candidates;
		vkEnumeratePhysicalDevices(m_vkInstance, &udeviceCount, nullptr);
		if (udeviceCount == 0)
			throw std::runtime_error("there is no physical device");
		devices.reserve(udeviceCount);
		vkEnumeratePhysicalDevices(m_vkInstance, &udeviceCount, devices.data());

		for (auto& device : devices)
		{
			if (isDeviceSuitable(device))
			{
				pVkPhydevice = device;
				break;
			}
		}
		if (pVkPhydevice == nullptr)
			throw std::runtime_error("no device suitable");
	}

	void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		pickPhysicalDevice();
	}

	void mainloop()
	{
		while (!glfwWindowShouldClose(m_pWindows))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		vkDestroyInstance(m_vkInstance, nullptr);
		if (m_pWindows)
			glfwDestroyWindow(m_pWindows);
		glfwTerminate();

	}
};


int main()
{
	HelloTriangleApplication app;
	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::println("run error {}",e.what());
		return -1;
	}
	return 0;
}