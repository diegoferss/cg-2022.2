#include "geometry/TriangleMesh.h"
#include "graph/PrimitiveProxy.h"
#include "graphics/TriangleMeshMapper.h"

class SpiralProxy final : public cg::graph::PrimitiveProxy {
 public:
  static auto New(const cg::TriangleMesh& mesh, const std::string& meshName) {
    return new SpiralProxy{mesh, meshName};
  }

  const char* const meshName() const { return _meshName.c_str(); }

  void setMesh(const cg::TriangleMesh& mesh, const std::string& meshName) {
    ((cg::TriangleMeshMapper*)PrimitiveProxy::mapper())->setMesh(mesh);
    _meshName = meshName;
  }

 private:
  std::string _meshName;

  SpiralProxy(const cg::TriangleMesh& mesh, const std::string& meshName)
      : PrimitiveProxy{*new cg::TriangleMeshMapper{mesh}}, _meshName{meshName} {
    // do nothing
  }
};