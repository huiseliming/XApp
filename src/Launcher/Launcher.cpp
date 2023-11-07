#include "App/App.h"

class CExampleApp : public CApp{
public:
  virtual void Tick() override {
    bool show_demo_window = true;
    if (show_demo_window)
      ImGui::ShowDemoWindow(&show_demo_window);
  }
};

int main(int argc, char *argv[]) { return CExampleApp().Run(argc, argv); }
