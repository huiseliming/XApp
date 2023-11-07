#include "GLImGuiLayer.h"
#include "GLRenderer.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void CGLImGuiLayer::Init(GLFWwindow *main_window, IRenderer *renderer) {
  MainWindow = main_window;
  Renderer = renderer;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
  io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(MainWindow, true);
  ImGui_ImplOpenGL3_Init(nullptr);
}

void CGLImGuiLayer::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void CGLImGuiLayer::DrawFrame() { CImGuiLayer::DrawFrame(); }

void CGLImGuiLayer::RenderFrame() {
  ImGuiIO &io = ImGui::GetIO();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
  }
}

void CGLImGuiLayer::Shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
