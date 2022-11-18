#include "our_GLGraphics.h"

#include "core/SharedObject.h"
#include "generatrix.h"
#include "mesh_sweeps.h"

cg::TriangleMesh* OurGLGraphics::spiral() {
  static cg::Reference<cg::TriangleMesh> _spiral;

  cg::Generatrix generatrix{20};

  if (_spiral == nullptr)
    _spiral = cg::MakeSpiral(generatrix, 40, 2, 2, 3, 0.01, nullptr);
  return _spiral;
}

cg::TriangleMesh* OurGLGraphics::twist() {
  static cg::Reference<cg::TriangleMesh> _twist;

  cg::Generatrix generatrix{20};

  if (_twist == nullptr)
    _twist = cg::MakeTwist(generatrix, 40, 2, 2, 3, 0.01, 2, 3, nullptr);
  return _twist;
}