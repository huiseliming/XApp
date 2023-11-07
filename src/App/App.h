#pragma once
#include "Renderer/Renderer.h"

class CApp;

XAPP_API extern CApp *GApp;

enum class ERendererType {
  Null,
  OpenGL,
  Vulkan,
};

class XAPP_API CApp {
public:
  CApp();
  ~CApp();

public:
  virtual int Run(int argc = 0, char *argv[] = nullptr);

protected:
  virtual bool Init();
  virtual void Loop();
  virtual void Exit();
 
  void InitRendererType();

public:
  GLFWwindow *GetMainWindow() { return MainWindow; }
  IRenderer *GetRenderer() { return Renderer; }
  ERendererType GetRendererType() { return RendererType; }

protected:
  GLFWwindow *MainWindow = nullptr;
  IRenderer *Renderer = nullptr;
  ERendererType RendererType{ERendererType::Vulkan};

private:
  bool CanExitLoop();
  int ExitCode = 0;
  bool bRequiredExitLoop = false;
  std::vector<std::function<bool()>> ExitLoopCheckers;

private:
  bool ParseArgs(int argc, char *argv[]);
  cxxopts::ParseResult Args;
};

#define IMPL_IMAPP(App)                                                        \
  int main(int argc, char *argv[]) { return App().Run(argc, argv); }
