#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

// glfw
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Core/Object.h"

class IRenderer;

namespace Utils {
XRENDERER_API std::vector<char>
LoadBinaryFromFile(const std::string &file_path);
XRENDERER_API std::string LoadCodeFromFile(const std::string &file_path);
} // namespace Utils
