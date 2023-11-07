#pragma once
#include "ImGuiLayer.h"
#include "Utils.h"

struct XRENDERER_API IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual void Init(GLFWwindow *main_window) = 0;
  virtual void Render() = 0;
  virtual void Shutdown() = 0;

  CImGuiLayer *GetImGuiLayer() { return ImGuiLayer; }

protected:
  glm::vec4 ClearColor = {
      0.0f,
      0.0f,
      0.0f,
      1.0f,
  };
  glm::vec3 CameraPosition;
  glm::vec3 CameraForward;
  glm::vec3 CameraUp;

  GLFWwindow *MainWindow{nullptr};
  CImGuiLayer *ImGuiLayer{nullptr};

private:
  friend class CImGuiLayer;
};