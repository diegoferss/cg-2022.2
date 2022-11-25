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
// OVERVIEW: MainWindow.h
// ========
// Class definition for cg demo main window.
//
// Author: Paulo Pagliosa
// Last revision: 22/07/2022

#ifndef __MainWindow_h
#define __MainWindow_h

#include "RayTracer.h"
#include "graph/SceneWindow.h"
#include "graphics/Assets.h"
#include "graphics/GLImage.h"
#include "spiral_proxy.h"
#include "twist_proxy.h"

using namespace cg;
using namespace cg::graph;

/////////////////////////////////////////////////////////////////////
//
// MainWindow: cg demo main window class
// ==========
class MainWindow final : public SceneWindow {
 public:
     friend class OurGLGraphics;

  MainWindow(int width, int height)
      : SceneWindow{"Ds Demo Version 1.1", width, height} {
    registerInspectFunction<SpiralProxy>(inspectSpiral);
    registerInspectFunction<TwistProxy>(inspectTwist);
  }

 private:
  Reference<RayTracer> _rayTracer;
  Reference<GLImage> _image;
  int _maxRecursionLevel{6};
  float _minWeight{RayTracer::minMinWeight};
  
  static const float* _num_subdiv{ new float};
  static const float* _spiral_initial_radius{ 2 * 2 };
  static const float* _spiral_num_revolutions{ 2 };
  static const float* _spiral_height_inc{ 2.5f * 2 };
  static const float* _spiral_radius_inc{ 0.01 };
  static const bool* _spiral_draw_front_cap{ true };
  static const bool* _spiral_draw_back_cap{ true };
  static const bool* _spiral_draw_generatrices{ false };

  static MeshMap _defaultMeshes;

  auto makeDefaultPrimitive(const char* const meshName) {
    auto object = makePrimitive(*_defaultMeshes[meshName], meshName);
    return object;
  }

  auto createDefaultPrimitiveObject(const char* const meshName) {
    auto object = createPrimitiveObject(*_defaultMeshes[meshName], meshName);
    return object;
  }

  void beginInitialize() override;
  void initializeScene() override;
  void renderScene() override;
  Component* addComponentMenu() override;
  void createObjectMenu() override;
  bool onResize(int width, int height) override;
  void gui() override;

  void mainMenu();
  void fileMenu();
  void createMenu();
  void showOptions();
  void ExportWavefront();

  void readScene(const std::string& filename);

  static void buildDefaultMeshes();

  static void inspectSpiral(SceneWindow&, SpiralProxy&);
  static void inspectTwist(SceneWindow&, TwistProxy&);

};  // MainWindow

#endif  // __MainWindow_h
