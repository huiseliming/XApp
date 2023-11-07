#include "App.h"

#include "Core/Fwd.h"
#include "Renderer/OpenGL/GLRenderer.h"
#include "Renderer/Vulkan/VKRenderer.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

static void GLFWErrorCallback(int error, const char *description) {
  SPDLOG_ERROR("GLFW Error {}: {}\n", error, description);
}

CApp *GApp = nullptr;

CApp::CApp() { GApp = this; }

CApp::~CApp() {}

int CApp::Run(int argc, char *argv[]) {
  GApp = this;
  try {
#if CK_DEBUG
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e [%t] %s:%# %! %v");
    spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e [%t] %v");
    spdlog::set_level(spdlog::level::info);
#endif
    SPDLOG_TRACE("");
    if (ParseArgs(argc, argv))
      return ExitCode;
    Init();
    while (!CanExitLoop()) {
      Loop();
    }
    Exit();
  } catch (const std::exception &exception) {
    std::cout << "CATCH EXCEPTION: " << exception.what() << std::endl;
    return -1;
  }
  return ExitCode;
}

void CApp::Tick() {
    if (Renderer)
    {
        Renderer->GetImGuiLayer()->DrawFrame();
    }
}

bool CApp::Init() {
  SPDLOG_TRACE("");
  InitRendererType();
  if (RendererType != ERendererType::Null) {
    glfwSetErrorCallback(GLFWErrorCallback);
    X_ASSERT(glfwInit());
    if (RendererType == ERendererType::Vulkan) {
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      MainWindow = glfwCreateWindow(1280, 720, "XApp", nullptr, nullptr);
      X_ASSERT(glfwVulkanSupported(), "GLFW: Vulkan Not Supported");
      Renderer = new CVKRenderer();
    } else if (RendererType == ERendererType::OpenGL) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
      MainWindow = glfwCreateWindow(1280, 720, "XApp", nullptr, nullptr);
      Renderer = new CGLRenderer();
    } else {
      X_NEVER_EXECUTED();
    }
    Renderer->Init(MainWindow);
  }
  return true;
}

void CApp::Loop() {
  if (Renderer) {
    glfwPollEvents();
    if (RendererType == ERendererType::Vulkan) {
      static_cast<CVKRenderer *>(Renderer)->RebuildSwapchain();
    }
    Renderer->GetImGuiLayer()->NewFrame();
    Tick();
    Renderer->Render();
  } else {
    Tick();
  }
}

void CApp::Exit() {
  SPDLOG_TRACE("");
  if (RendererType != ERendererType::Null) {
    // destroy sdl renderer
    Renderer->Shutdown();
    delete Renderer;
    glfwDestroyWindow(MainWindow);
    glfwTerminate();
  }
}

void CApp::InitRendererType()
{
    if (Args.count("rhi") > 0)
    {
        std::string renderer_type = Args["rhi"].as<std::string>();
        if (renderer_type == "null") {
            RendererType = ERendererType::Null;
        }
        else if (renderer_type == "opengl" || renderer_type == "gl") {
            RendererType = ERendererType::OpenGL;
        }
        else if (renderer_type == "vulkan" || renderer_type == "vk") {
            RendererType = ERendererType::Vulkan;
        }
        else {
        }
        return;
    }
    if (Args["vk"].as<bool>()) {
        RendererType = ERendererType::Vulkan;
        return;
    }
    if (Args["gl"].as<bool>()) {
        RendererType = ERendererType::OpenGL;
        return;
    }
}

bool CApp::CanExitLoop() {
  if (glfwWindowShouldClose(MainWindow)) {
    ExitLoopCheckers.erase(std::remove_if(ExitLoopCheckers.begin(),
                                          ExitLoopCheckers.end(),
                                          [](auto &checker) {
                                            if (checker())
                                              return true;
                                            return false;
                                          }),
                           ExitLoopCheckers.end());
    if (ExitLoopCheckers.empty()) {
      return true;
    }
  }
  return false;
}

bool CApp::ParseArgs(int argc, char *argv[]) {
  cxxopts::Options Options("opts", "parse opts");
  Options.add_options()
      ("console", " ", cxxopts::value<bool>()->default_value("false"))
      ("rhi", "use null/vk(vulkan)/gl(opengl)", cxxopts::value<std::string>())
      ("gl", "use vulkan", cxxopts::value<bool>()->default_value("false"))
      ("vk", "use vulkan", cxxopts::value<bool>()->default_value("false"))
      ("version", "print version")("h,help", "print usage");
  Args = Options.parse(argc, argv);
  if (Args.count("help")) {
    std::cout << Options.help() << std::endl;
    return true;
  }
  return false;
}
