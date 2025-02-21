#ifndef QUEUE_FAMILY_H
#define QUEUE_FAMILY_H

#include <vulkan/vulkan.h>
#include <optional>
#include <iostream>
#include <vector>

class QueueFamilyIndices {

public:

	QueueFamilyIndices() = default;

	bool isComplete() const {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}

	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

};

class SwapChainSupportDetails {

public:

	SwapChainSupportDetails() = default;

	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


#endif