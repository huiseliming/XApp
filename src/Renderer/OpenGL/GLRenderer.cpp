#include "GLRenderer.h"
#include "GLImGuiLayer.h"

void CGLRenderer::Init(GLFWwindow *main_window) {
  MainWindow = main_window;
  glfwMakeContextCurrent(MainWindow);
  X_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  glfwSwapInterval(1);

  ImGuiLayer = new CGLImGuiLayer();
  ImGuiLayer->Init(MainWindow, this);
}

void CGLRenderer::Render() {
  int display_w, display_h;
  glfwGetFramebufferSize(MainWindow, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGuiLayer->RenderFrame();

  glfwSwapBuffers(MainWindow);
}

void CGLRenderer::Shutdown() {
  ImGuiLayer->Shutdown();

}
