#include "GUI.h"

void
GUI::testWindow(const char* title)
{
    if (!_windowTest)
        return;
    assert(title != nullptr);
    ImGui::Begin(title);
    ImGui::SliderFloat("Speed", a, 100.0f, 1000.0f);
    ImGui::End();
}