//
// Created by Theo Bessel on 2019-03-25.
//

#include "mainWindow.hpp"
#include "../../usual/logger.hpp"
#include "../../usual/files.hpp"
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN
#include <glm/glm.hpp>
#include <optional>
#include <set>
#include <fstream>
#include <array>

//
// Define a structure to save Vertex data
//
struct Vertex {
    glm::vec3 pos;          // Position of vertex
    glm::vec3 color;        // Color of vertex

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};



//
// Vertex rendered initialisation
//
std::vector<Vertex> vertices = {
        /*{ {-1.0, -1.0, 1.0}, {0, 0, 1} },
        { { 1.0, -1.0, 0.0}, {1, 0, 1} },
        { { 1.0,  1.0, 0.0}, {1, 0, 0} },
        { {-1.0,  1.0, 0.0}, {1, 0, 1} },
        { {-1.0, -1.0, 1.0}, {0, 0, 1} },*/
        {{-1, 1, 0}, {0, 0, 0}},
        {{-1,-1, 0}, {0, 0, 1}},
        {{ 1,-1, 0}, {1, 0, 1}},
        {{ 1, 1, 0}, {1, 0, 0}},
        {{-1, 1, 0}, {0, 0, 0}}
};

//
// Create a debug messenger
//
VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(
                instance,
                pCreateInfo,
                pAllocator,
                pDebugMessenger
            );
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

//
// Destroy the debug messenger
//
void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        func(
            instance,
            debugMessenger,
            pAllocator
        );
    }
}

//
// Init a window using GLFW
//
void MainWindow::initWindow() {
    // Initialisation

    glfwInit();                                     // Init GLFW
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // GLFW API options
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // GLFW window options

    if (glfwVulkanSupported()) {                    // Check if Vulkan is supported by GLFW
        usual::clog("Vulkan supported by GLFW !\n");
    } else {
        throw std::runtime_error("Vulkan NOT supported by GLFW !");
    }

    // Create window

    window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "Render window",
        nullptr,
        nullptr
    );                                  // Create an empty GLFW window

    glfwSetWindowUserPointer(
        window, 
        this
    );                                  // Set the user pointer to window pointer

    glfwSetFramebufferSizeCallback(
        window, 
        framebufferResizeCallback
    );                                  // Use Callback function to handle resize
}

//
// Handle GLFW window resize
//
void MainWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

//
// Init vulkan in two step : device config first, render config then
//
void MainWindow::initVulkan() {
    // First Step : device config
    createInstance();           // Create Vulkan Instance
    setupDebugMessenger();      // Setup Vulkan messenger in debug mode
    createSurface();            // Create Render Surface and apply it to GLFW window
    pickPhysicalDevice();       // Chose the first suitable physical device (TODO: Rate the device suitability)
    createLogicalDevice();      // Create a Logical Device which is an interface to control a Physical Device

    // Second Step : render config
    createSwapChain();          // Create a SwapChain (a queue where images are stocked before the render)
    createImageViews();         // Create views in the image (target of what will be colored)
    createRenderPass();         // Create RenderPass, a struct that contains informations about Framebuffers :
                                //      (number of color and depth buffers, number of samples and how to use them)

    createGraphicsPipeline();   // Create Graphic Pipeline steps informations
    createFramebuffers();       // Create a Frame Buffer (a collection of Image Views used by Render Pass)
    createCommandPool();        // Create a Command Pool that manage memory used by Command Buffers
    createVertexBuffer();       // Create a Vertex Buffer that contains Rendered Vertex and is stocked in GPU memory
    createCommandBuffers();     // Create Command Buffers that repertory all the operations to execute
                                //      Command Buffers can be used in many different threads and are executed from queues

    createSyncObjects();        // Create Semaphores that synchronize the differents steps of SwapChain
}

//
// Loop to let the GLFW window opened until the close event
//
void MainWindow::mainLoop() {
    while (!glfwWindowShouldClose(window)) { // Let the window open until close event
        glfwPollEvents();                    // Collect GLFW events
        drawFrame();                         // Draw frame
    }
    vkDeviceWaitIdle(device);                // Wait
}

//
// Clean all the SwapChain (a queue where images are stocked before the render)
//
void MainWindow::cleanupSwapChain() {
    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(
                device,
                swapChainFramebuffers[i],
                nullptr
        );
    }       // Destroy all the Frame Buffers

    vkFreeCommandBuffers(
            device,
            commandPool,
            static_cast<uint32_t>(
                commandBuffers.size()
            ),
            commandBuffers.data()
    );      // Clean the Command Buffer

    vkDestroyPipeline(
        device,
        graphicsPipeline,
        nullptr
    );      // Destroy Graphic Pipeline

    vkDestroyPipelineLayout(
        device,
        pipelineLayout,
        nullptr
    );      // Destroy Graphic Pipeline Layout (an object where all the setup for Graphic Pipeline is done)
    
    vkDestroyRenderPass(
        device,
        renderPass,
        nullptr
    );      // Destroy Render Pass (a struct that contains informations about Framebuffers)

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vkDestroyImageView(
            device,
            swapChainImageViews[i],
            nullptr
        );  // Destroy all the Image Views (target of what will be colored)
    }

    vkDestroySwapchainKHR(
        device,
        swapChain,
        nullptr
    );      // Destroy the SwapChain object
}

//
// Clean all the non used objects when window is closed (used in run() method, cf. mainWindow.hpp)
//
void MainWindow::cleanup() {
    cleanupSwapChain();         // Clean Swapchain

    vkDestroyBuffer(
        device,
        vertexBuffer,
        nullptr
    );                          // Destroy Vertex Buffer

    vkFreeMemory(
        device,
        vertexBufferMemory,
        nullptr
    );                          // Free memory used by Vertex Buffer

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(
            device,
            renderFinishedSemaphores[i],
            nullptr
        );                      // Destroy Finished Semaphores

        vkDestroySemaphore(
            device,
            imageAvailableSemaphores[i],
            nullptr
        );                      // Destroy Available Semaphores

        vkDestroyFence(
            device,
            inFlightFences[i],
            nullptr
        );                      // Destroy Fences
    }

    vkDestroyCommandPool(
        device,
        commandPool,
        nullptr
    );                          // Dessstroy command pool object

    vkDestroyDevice(
        device,
        nullptr
    );                          // Desstroy Vulkan Device instance

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(
            instance,
            debugMessenger,
            nullptr
        );                      // Destroy debug messenger
    }

    vkDestroySurfaceKHR(
        instance,
        surface,
        nullptr
    );                          // Destroy Render Surface

    vkDestroyInstance(
        instance,
        nullptr
    );                          // Destroy Vulkan instance

    glfwDestroyWindow(window);  // Destroy GLFW window object

    glfwTerminate();            // Terminate GLFW
}

//
// Create Vulkan instance with informations about its version, the extension and the validation Layers
//
void MainWindow::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation Layers requested, but not available !");
    }

    VkApplicationInfo appInfo = {};                                                     // Application info structure
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Web Render";                                            // Application name
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);                              // Application version
    appInfo.pEngineName = "No Engine";                                                  // Engine name
    appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);                                   // Engine version
    appInfo.apiVersion = VK_API_VERSION_1_0;                                            // Vulkan API version

    auto extensions = getRequiredExtensions();                                          // Get all extension required by the application
    usual::clog("Required Extensions : ", extensions.size());

    VkInstanceCreateInfo createInfo = {};                                               // Vulkan info structure
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;                                             // Vulkan application info
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());        // Numbert of enabled extentions
    createInfo.ppEnabledExtensionNames = extensions.data();                             // Names of enabled extensions

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());  // Number of validation layers
        createInfo.ppEnabledLayerNames = validationLayers.data();                       // Names of validation layers
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance !");
    }
}

//
// Setup a messenger for debug error messages
//
void MainWindow::setupDebugMessenger() {
    if(!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
                                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
                                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
                            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
                            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;

    if (CreateDebugUtilsMessengerEXT(
            instance,
            &createInfo,
            nullptr,
            &debugMessenger
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to set up debug messenger !");
    }
}

//
// Create a render surface in the GLFW window
//
void MainWindow::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        usual::clog("Error : ", 
                    glfwCreateWindowSurface(
                        instance,
                        window,
                        nullptr,
                        &surface
                    ));
        throw std::runtime_error("Failed to create window surface !");
    }
}

//
// Pick all the suitable Physical Devices
//
void MainWindow::pickPhysicalDevice() {
    uint32_t deviceCount = 0;                           // Couter for the number of Physical Devices

    vkEnumeratePhysicalDevices(
        instance,
        &deviceCount,
        nullptr
    );                                                  // Count the devices into deviceCount

    if(deviceCount == 0) {
        throw std::runtime_error("Failed to find GPU(s) with Vulkan support !");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount); // Array to stock the different Physical Devices
    vkEnumeratePhysicalDevices(
        instance,
        &deviceCount,
        devices.data()
    );                                                  // Append the devices into Devices

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {                 // Check for every device if it's suitable
            physicalDevice = device;
            break;
        }
    } if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU !");
    }
}

//
// Obtain indices for all suitable Queue Families
//
QueueFamilyIndices MainWindow::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;                 // Store each Queue Family indice

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queueFamilyCount,
        nullptr
    );

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(   // Obtain properties for each device Queue Family
        device,
        &queueFamilyCount,
        queueFamilies.data()
    );

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(   // Test if device support KHR (Khronos Vulkan)
            device,
            i,
            surface,
            &presentSupport
        );

        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }
        i++;
    }
    return indices;
}

//
// Create a Logical Device : the interface for Physical Device
//
void MainWindow::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice); // Find the queue families for the used Physical Device

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {              // Set the informations for each family
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    //vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkPhysicalDeviceFeatures available;

    vkGetPhysicalDeviceFeatures(
        physicalDevice,
        &available
    );                                                              // Get all the features available with the Physical Device

    if (available.wideLines == VK_TRUE)
        deviceFeatures.wideLines = VK_TRUE;                         // Activate the feature : wideLines

    if (available.largePoints == VK_TRUE)
        deviceFeatures.largePoints = VK_TRUE;                       // Activate the feature : largePoints

    VkDeviceCreateInfo createInfo = {};                             // Set the informations about the Logical Device
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {                                   // Activate the validation layers for the Logical Device
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }
    if (vkCreateDevice(
            physicalDevice,
            &createInfo,
            nullptr,
            &device
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device !");
    }

    vkGetDeviceQueue(
        device,
        indices.graphicsFamily.value(),
        0,
        &graphicsQueue
    );                                                              // Queue Family supporting graphic commands

    vkGetDeviceQueue(
        device,
        indices.presentFamily.value(),
        0,
        &presentQueue
    );                                                              // Queue Family supporting presentation commands
}

//
// SwapChain (a queue where images are stocked before the render) creation process
//
void MainWindow::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);       // Get all the informations about supported SwapChains

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);   // Select the good format for the SwapChain 
                                                                                            //      (window size + color palette)

    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);    // Select the good presentation mode

    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);                    // 

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(
            device,
            &createInfo,
            nullptr,
            &swapChain
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swap chain !");
    }

    vkGetSwapchainImagesKHR(
        device,
        swapChain,
        &imageCount,
        nullptr
    );

    swapChainImages.resize(imageCount);

    vkGetSwapchainImagesKHR(
        device,
        swapChain,
        &imageCount,
        swapChainImages.data()
    );

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

//
// 
//
void MainWindow::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {

        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(
                device,
                &createInfo,
                nullptr,
                &swapChainImageViews[i]
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create image views !");
        }
    }
}

//
// Compile shader files
//
void MainWindow::compileShaders() {
    system("../../render/mainWindow/compile.sh");
}

//
// 
//
void MainWindow::createRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT 
                                | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(
            device,
            &renderPassInfo,
            nullptr,
            &renderPass
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render pass !");
    }
}

// compile with using
// cd /Users/theobessel/Desktop/WebRender/render/mainWindow
// && glslangValidator -V fragment/baseShader.frag
// && glslangValidator -V vertex/baseShader.vert

//
// 
//
void MainWindow::createGraphicsPipeline() {
    compileShaders();
    auto vertShaderCode = usual::readFileToBuffer("/Users/theobessel/Programmation/AtherisWeb/render/mainWindow/vertex/vert.spv");
    auto fragShaderCode = usual::readFileToBuffer("/Users/theobessel/Programmation/AtherisWeb/render/mainWindow/fragment/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertShaderStageInfo,
        fragShaderStageInfo
    };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;//VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; //VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;                                                  // Viewport x position
    viewport.y = 0.0f;                                                  // Viewport y position
    viewport.width = static_cast<float>(swapChainExtent.width);         // Viewport x size
    viewport.height = static_cast<float>(swapChainExtent.height);       // Viewport y size
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT 
                                        | VK_COLOR_COMPONENT_G_BIT 
                                        | VK_COLOR_COMPONENT_B_BIT 
                                        | VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    if (vkCreatePipelineLayout(
            device,
            &pipelineLayoutInfo,
            nullptr,
            &pipelineLayout
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout !");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(
            device,
            VK_NULL_HANDLE,
            1,
            &pipelineInfo,
            nullptr,
            &graphicsPipeline
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics pipeline !");
    }

    vkDestroyShaderModule(
        device,
        fragShaderModule,
        nullptr
    );

    vkDestroyShaderModule(
        device,
        vertShaderModule,
        nullptr
    );
}

//
// 
//
void MainWindow::createFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(
                device,
                &framebufferInfo,
                nullptr,
                &swapChainFramebuffers[i]
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create framebuffer !");
        }
    }
}

//
// 
//
void MainWindow::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = 0;

    if (vkCreateCommandPool(
            device,
            &poolInfo,
            nullptr,
            &commandPool
        ) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool !");
    }
}

//
// 
//
void MainWindow::createVertexBuffer() {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = (sizeof(vertices[0])) * (vertices.size());
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(
            device,
            &bufferInfo,
            nullptr,
            &vertexBuffer
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create vertex buffer !");
    }

    VkMemoryRequirements memRequirements;

    vkGetBufferMemoryRequirements(
        device,
        vertexBuffer,
        &memRequirements
    );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    if (vkAllocateMemory(
            device,
            &allocInfo,
            nullptr,
            &vertexBufferMemory
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate vertex buffer memory !");
    }

    vkBindBufferMemory(
        device,
        vertexBuffer,
        vertexBufferMemory,
        0
    );
    
    void* data;

    vkMapMemory(
        device,
        vertexBufferMemory,
        0,
        bufferInfo.size,
        0,
        &data
    );

    memcpy(
        data,
        vertices.data(),
        static_cast<size_t>(bufferInfo.size)
    );
    
    vkUnmapMemory(
        device,
        vertexBufferMemory
    );
}

//TODO : Create Uniform Buffer

//
// 
//
uint32_t MainWindow::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;

    vkGetPhysicalDeviceMemoryProperties(
        physicalDevice,
        &memProperties
    );

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    } throw std::runtime_error("Failed to find suitable memory type !");
}

//
// 
//
void MainWindow::createCommandBuffers() {
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(
            device,
            &allocInfo,
            commandBuffers.data()
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers !");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(
                commandBuffers[i],
                &beginInfo
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to begin recording command buffer !");
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent;

        VkClearValue clearColor = {1,1,1}; //{0.21176471f, 0.22352941f, 0.24313725f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(
            commandBuffers[i],
            &renderPassInfo,
            VK_SUBPASS_CONTENTS_INLINE
        );
        
        vkCmdBindPipeline(
            commandBuffers[i],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            graphicsPipeline
        );

        VkBuffer vertexBuffers[] = {
            vertexBuffer
        };

        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(
            commandBuffers[i],
            0,
            1,
            vertexBuffers,
            offsets
        );

        vkCmdDraw(
            commandBuffers[i],
            static_cast<uint32_t>(vertices.size()),
            1,
            0,
            0
        );

        //vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices2.size()), 1, 0, 0);

        vkCmdBindPipeline(
            commandBuffers[i],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            graphicsPipeline
        );

        vkCmdDraw(
            commandBuffers[i],
            vertices.size(),
            1,
            0,
            0
        );

        //vkCmdDraw(commandBuffers[i], vertices2.size(), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer !");
        }
    }
}

//
// 
//
void MainWindow::drawFrame() {
    vkWaitForFences(
        device,
        1,
        &inFlightFences[currentFrame],
        VK_TRUE,
        std::numeric_limits<uint64_t>::max()
    );

    uint32_t imageIndex;

    VkResult result = vkAcquireNextImageKHR(
                        device,
                        swapChain,
                        std::numeric_limits<uint64_t>::max(),
                        imageAvailableSemaphores[currentFrame],
                        VK_NULL_HANDLE,
                        &imageIndex
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image !");
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {
        imageAvailableSemaphores[currentFrame]
    };
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {
        renderFinishedSemaphores[currentFrame]
    };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(
        device,
        1,
        &inFlightFences[currentFrame]
    );

    if (vkQueueSubmit(
            graphicsQueue,
            1,
            &submitInfo,
            inFlightFences[currentFrame]
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit draw command buffer !");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {
        swapChain
    };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(
                presentQueue,
                &presentInfo
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image !");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

//
// 
//
void MainWindow::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(
                device,
                &semaphoreInfo,
                nullptr,
                &imageAvailableSemaphores[i]
            ) != VK_SUCCESS 
        ||  vkCreateSemaphore(
                device,
                &semaphoreInfo,
                nullptr,
                &renderFinishedSemaphores[i]
            ) != VK_SUCCESS 
        ||  vkCreateFence(
                device,
                &fenceInfo,
                nullptr,
                &inFlightFences[i]
            ) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create synchronization objects for a frame !");
        }
    }
}

//
// 
//
VkShaderModule MainWindow::createShaderModule(const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(
            device,
            &createInfo,
            nullptr,
            &shaderModule
        ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module !");
    } return shaderModule;
}

//
// 
//
VkSurfaceFormatKHR MainWindow::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {
            VK_FORMAT_B8G8R8A8_UNORM,
            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        };
    }
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM
        && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    } return availableFormats[0];
}

//
// 
//
VkPresentModeKHR MainWindow::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = availablePresentMode;
        }
    } return bestMode;
}

//
// 
//
VkExtent2D MainWindow::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;

        glfwGetFramebufferSize(
            window,
            &width,
            &height
        );

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(
            capabilities.minImageExtent.width,
            std::min(
                capabilities.maxImageExtent.width,
                actualExtent.width
            )
        );

        actualExtent.height = std::max(
            capabilities.minImageExtent.height,
            std::min(
                capabilities.maxImageExtent.height,
                actualExtent.height
            )
        );

        return actualExtent;
    }
}

//
// 
//
SwapChainSupportDetails MainWindow::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device,
        surface,
        &details.capabilities
    );

    uint32_t formatCount;
    
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device,
        surface,
        &formatCount,
        nullptr
    );

    if (formatCount != 0) {
        details.formats.resize(formatCount);

        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device,
            surface,
            &formatCount,
            details.formats.data()
        );
    }

    uint32_t presentModeCount;

    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device,
        surface,
        &presentModeCount,
        nullptr
    );

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);

        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device,
            surface,
            &presentModeCount,
            details.presentModes.data()
        );

    } return details;
}

//
// Check if a Physical Device can be used
//
bool MainWindow::isDeviceSuitable(VkPhysicalDevice device) {

    QueueFamilyIndices indices = findQueueFamilies(device);                         // Check if all the required Queue Families are supported

    bool extensionsSupported = checkDeviceExtensionSupport(device);                 // Check if required extensions are supported
    bool swapChainAdequate = false;

    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);   // Check if SwapChain is supported

        swapChainAdequate = !swapChainSupport.formats.empty() 
                            && !swapChainSupport.presentModes.empty();

    } return indices.isComplete() && extensionsSupported && swapChainAdequate;      // Device is suitable if all the required conditions are respected
}

//
// 
//
bool MainWindow::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;

    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &extensionCount,
        nullptr
    );

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &extensionCount,
        availableExtensions.data()
    );

    std::set<std::string> requiredExtensions(
        deviceExtensions.begin(),
        deviceExtensions.end()
    );

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    } return requiredExtensions.empty();
}

//
// Get all extension required by the application
//
std::vector<const char*> MainWindow::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;                                // Count the extensions

    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(
        &glfwExtensionCount
    );                                                              // Get all required extension

    std::vector<const char*> extensions(
        glfwExtensions,
        glfwExtensions + glfwExtensionCount
    );

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);    // Push the extension if required
    } return extensions;
}

//
//  Return all the available validation layers
//
bool MainWindow::checkValidationLayerSupport() {

    uint32_t layerCount;

    vkEnumerateInstanceLayerProperties(
        &layerCount,
        nullptr
    );                                                          // Enumerate all the layers in instance

    usual::clog("Layers : ", layerCount);

    std::vector<VkLayerProperties> availableLayers{
        layerCount
    };

    vkEnumerateInstanceLayerProperties(
        &layerCount,
        availableLayers.data()
    );                                                          // Enumerate all the validation layers

    usual::clog("Available Layers : ", layerCount);             // Print all the available validation layers

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(
                    layerName,
                    layerProperties.layerName
                ) == 0) {
                    layerFound = true;
                break;
            }
        } if (!layerFound) {
            return false;
        }
    }
    return true;
}

//
// Refresh the SwapChain when window is resized
//
void MainWindow::recreateSwapChain() {
    int width = 0, height = 0;

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    cleanupSwapChain();         // Destroy the SwapChain
    createSwapChain();          // Create a new SwapChain
    createImageViews();         // Create new ImageViews
    createRenderPass();         // Create a new RenderPass
    createGraphicsPipeline();   // Create a new Graphic Pipeline
    createFramebuffers();       // Create new Framebuffers
    createCommandBuffers();     // Create new Command Buffers
}

//
// Log validation layers errors
//
VKAPI_ATTR VkBool32 VKAPI_CALL MainWindow::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
        usual::clog("Validation layer : ", pCallbackData->pMessage);
        return VK_FALSE;
}