#pragma once
#include "../Renderer.h"
#include "GLUtils.h"

struct XRENDERER_API CGLRenderer : public IRenderer {
  virtual void Init(GLFWwindow *main_window) override;
  virtual void Render() override;
  virtual void Shutdown() override;

private:
  friend class CGLImGuiLayer;
};
