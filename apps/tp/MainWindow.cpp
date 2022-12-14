//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2022 Paulo Pagliosa.                              |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: MainWindow.cpp
// ========
// Source file for cg demo main window.
//
// Author: Paulo Pagliosa
// Last revision: 11/03/2022

#include "MainWindow.h"

#include <cstdio>

#include "graph/Component.h"
#include "graphics/Application.h"
#include "nfd.h"
#include "obj_writer.h"
#include "our_GLGraphics.h"
#include "reader/SceneReader.h"

#ifdef _DEBUG
#define NFD_CHECK(result)                                   \
  {                                                         \
    if ((result) == NFD_ERROR)                              \
      fprintf_s(stderr, "NFD Error: %s\n", NFD_GetError()); \
  }
#else
#define NFD_CHECK(result) (result)
#endif  // _DEBUG

/////////////////////////////////////////////////////////////////////
//
// MainWindow implementation
// ==========
MeshMap MainWindow::_defaultMeshes;

inline void MainWindow::buildDefaultMeshes() {
  _defaultMeshes["Box"] = GLGraphics3::box();
  _defaultMeshes["Sphere"] = GLGraphics3::sphere();
  _defaultMeshes["Cylinder"] = GLGraphics3::cylinder();
  _defaultMeshes["Cone"] = GLGraphics3::cone();
  _defaultMeshes["Spiral"] = OurGLGraphics::spiral();
  _defaultMeshes["Twist"] = OurGLGraphics::twist();
}

void MainWindow::initializeScene() {
  auto a = (float)width() / (float)height();
  auto s = createCameraObject(a, "Main Camera");

  s->transform()->setLocalPosition({0, 0, 10});
  s = createLightObject(Light::Type::Directional, "Directional Light");
  s->transform()->setLocalEulerAngles({50, 30, 0});
  // createDefaultPrimitiveObject("Spiral");
}

void MainWindow::beginInitialize() {
  Assets::initialize();
  buildDefaultMeshes();
  Assets::meshes().insert(_defaultMeshes.begin(), _defaultMeshes.end());

  /*constexpr auto ffn = "fonts/Roboto-Regular.ttf";
  auto fonts = ImGui::GetIO().Fonts;

  fonts->AddFontFromFileTTF(Application::assetFilePath(ffn).c_str(), 16);*/
}

namespace cg {

class SpiralParameters final : public Component {
 public:
  SpiralParameters() : Component{"Parameters", false} { subdiv = 3; }

  void afterAdded() override {}

  void update() override {}

  void beforeRemoved() override {}

  float subdiv;
};

}  // namespace cg

void MainWindow::inspectSpiral(SceneWindow&, SpiralProxy& proxy) {}

void MainWindow::inspectTwist(SceneWindow&, TwistProxy& proxy) {}

void MainWindow::createObjectMenu() {
  if (ImGui::BeginMenu("3D Object")) {
    if (ImGui::MenuItem("Box")) createDefaultPrimitiveObject("Box");
    if (ImGui::MenuItem("Sphere")) createDefaultPrimitiveObject("Sphere");
    if (ImGui::MenuItem("Cylinder")) createDefaultPrimitiveObject("Cylinder");
    if (ImGui::MenuItem("Cone")) createDefaultPrimitiveObject("Cone");
    if (ImGui::MenuItem("Spiral")) {
      auto obj = createDefaultPrimitiveObject("Spiral");
      auto tmp = new SpiralParameters();
      obj->addComponent(tmp);
    }
    if (ImGui::MenuItem("Twist")) createDefaultPrimitiveObject("Twist");
    ImGui::EndMenu();
  }
}

Component* MainWindow::addComponentMenu() {
  Component* component = nullptr;

  if (ImGui::BeginMenu("3D Object")) {
    if (ImGui::MenuItem("Box")) component = makeDefaultPrimitive("Box");
    if (ImGui::MenuItem("Sphere")) component = makeDefaultPrimitive("Sphere");
    if (ImGui::MenuItem("Cylinder"))
      component = makeDefaultPrimitive("Cylinder");
    if (ImGui::MenuItem("Cone")) component = makeDefaultPrimitive("Cone");
    if (ImGui::MenuItem("Spiral")) component = makeDefaultPrimitive("Spiral");
    if (ImGui::MenuItem("Plane")) component = makeDefaultPrimitive("Plane");
    ImGui::EndMenu();
  }
  return component;
}

void MainWindow::readScene(const std::string& filename) {
  try {
    parser::SceneReader reader;

    reader.setInput(filename);
    reader.execute();
    if (reader.scene() != nullptr) SceneWindow::setScene(*reader.scene());
  } catch (const std::exception& e) {
    puts(e.what());
  }
}

inline void MainWindow::fileMenu() {
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("New Scene")) newScene();
    if (ImGui::BeginMenu("Open")) {
      // TODO
      if (ImGui::MenuItem("test.scn"))
        readScene(Application::assetFilePath("scenes/test.scn"));
      ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Exit", "Alt+F4")) {
      shutdown();
    }
    ImGui::EndMenu();
  }
}

inline void MainWindow::createMenu() {
  if (ImGui::BeginMenu("Create")) {
    if (ImGui::MenuItem("Material")) createMaterial();
    ImGui::Separator();
    createObjectMenu();
    ImGui::EndMenu();
  }
}

inline void MainWindow::showOptions() {
  ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
  ImGui::showStyleSelector("Color Theme##Selector");
  ImGui::colorEdit3("Selected Wireframe", _selectedWireframeColor);
  ImGui::PopItemWidth();
}

inline void MainWindow::ExportWavefront() {
  char* usrprof_buf;
  _dupenv_s(&usrprof_buf, nullptr, "userprofile");
  nfdchar_t default_path[256];
  sprintf_s(default_path, "%s\\Desktop", usrprof_buf);
  free(usrprof_buf);
  nfdchar_t* out_path{};
  NFD_CHECK(NFD_SaveDialog(nullptr, default_path, &out_path));
  if (out_path) {
    std::FILE* out_handle{};
    fopen_s(&out_handle, out_path, "w");
    free(out_path);
    WriteMesh(_defaultMeshes["Spiral"], out_handle);
    if (out_handle) fclose(out_handle);
  }
}

inline void MainWindow::mainMenu() {
  if (ImGui::BeginMainMenuBar()) {
    fileMenu();
    createMenu();
    if (ImGui::BeginMenu("View")) {
      static const char* viewLabels[]{"Editor", "Ray Tracer"};

      if (ImGui::BeginCombo("View", viewLabels[(int)_viewMode])) {
        for (auto i = 0; i < IM_ARRAYSIZE(viewLabels); ++i) {
          if (ImGui::Selectable(viewLabels[i], _viewMode == (ViewMode)i))
            _viewMode = (ViewMode)i;
        }
        ImGui::EndCombo();
        // TODO: change mode only if scene has changed
        if (_viewMode == ViewMode::Editor) _image = nullptr;
      }
      ImGui::Separator();
      ImGui::MenuItem("Hierarchy Window", nullptr, &_showHierarchy);
      ImGui::MenuItem("Inspector Window", nullptr, &_showInspector);
      ImGui::MenuItem("Camera Preview", nullptr, &_showPreview);
      ImGui::MenuItem("Assets Window", nullptr, &_showAssets);
      ImGui::MenuItem("Editor View Settings", nullptr, &_showEditorView);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Ray Tracing")) {
      ImGui::DragInt("Max Recursion Level", &_maxRecursionLevel, 1.0f, 0,
                     RayTracer::maxMaxRecursionLevel);
      ImGui::DragFloat("Min Weight", &_minWeight, 0.01f,
                       RayTracer::minMinWeight, 1.0f);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools")) {
      if (ImGui::MenuItem("Export Wavefront")) ExportWavefront();
      if (ImGui::BeginMenu("Options")) {
        showOptions();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void MainWindow::gui() {
  mainMenu();
  if (_viewMode == ViewMode::Renderer) return;

  constexpr auto evw = (float)360;
  constexpr auto evh = (float)308;
  constexpr auto lft = (float)0;
  constexpr auto top = (float)23;
  auto w = (float)width();
  auto h = (float)height();
  auto evy = h - evh;

  // Hierarchy Window
  ImGui::SetNextWindowPos({lft, top});
  ImGui::SetNextWindowSize({evw, evy - top});
  hierarchyWindow();
  // Editor View
  ImGui::SetNextWindowPos({lft, evy});
  ImGui::SetNextWindowSize({evw, evh});
  editorView();

  constexpr auto iww = (float)420;
  auto rgt = w - iww;
  auto awh = h / 3;
  auto awy = h - awh;

  // Inspector Window
  ImGui::SetNextWindowPos({rgt, top});
  ImGui::SetNextWindowSize({iww, awh});
  inspectorWindow();


  // Nossas coisas
  ImGui::SetNextWindowPos({ rgt, h / 3 + top});
  ImGui::SetNextWindowSize({ iww, awh - top });
  ImGui::Begin("Nossas coisas");
      ImGui::SliderFloat("Spiral rev.", &_spiral_num_revolutions, 0, 10);
      ImGui::SliderFloat("Spiral height inc.", &_spiral_height_inc, 0, 10);
      ImGui::SliderFloat("Spiral adius inc.", &_spiral_radius_inc, 0, 10);
      ImGui::Checkbox("Front cap", &_spiral_draw_front_cap);
      ImGui::Checkbox("Front cap", &_spiral_draw_front_cap);
      ImGui::SameLine();
      ImGui::Checkbox("Back cap", &_spiral_draw_back_cap);
      ImGui::Checkbox("Back cap", &_spiral_draw_back_cap);
      ImGui::Checkbox("Draw generatrices", &_spiral_draw_generatrices);
      ImGui::Separator();
  ImGui::End();

  // Assets Window
  ImGui::SetNextWindowPos({rgt, awy});
  ImGui::SetNextWindowSize({iww, awh});
  assetsWindow();
}

void MainWindow::renderScene() {
  if (_viewMode != ViewMode::Renderer) return;

  auto camera = CameraProxy::current();

  if (nullptr == camera) camera = editor()->camera();
  if (_image == nullptr) {
    _image = new GLImage{width(), height()};
    if (_rayTracer == nullptr)
      _rayTracer = new RayTracer{*scene(), *camera};
    else
      _rayTracer->setCamera(*camera);
    _rayTracer->setMaxRecursionLevel(_maxRecursionLevel);
    _rayTracer->setMinWeight(_minWeight);
    _rayTracer->renderImage(*_image);
  }
  _image->draw(0, 0);
}

bool MainWindow::onResize(int width, int height) {
  _viewMode = ViewMode::Editor;
  _image = nullptr;
  return true;
}

#ifdef NFD_CHECK
#undef NFD_CHECK
#endif  // NFD_CHECK
