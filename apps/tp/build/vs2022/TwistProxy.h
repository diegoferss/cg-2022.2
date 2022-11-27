#include "graph/PrimitiveProxy.h"
#include "geometry/TriangleMesh.h"
#include "graphics/TriangleMeshMapper.h"

class TwistProxy final : public cg::graph::PrimitiveProxy
{
public:

    float _arc_angle = 360;
    bool _is_polygon = false;
    bool _arc_polyline_situation = false;
    int _generatrix_subdiv = 4;
    int _twist_num_subdiv = 40;
    float _twist_num_revolutions = 2;
    float _twist_length = 5;
    float _twist_vertical_pos = 0;
    float _twist_horiz_pos = 0;
    float _twist_initial_scale = 1;
    float _twist_final_scale = 1;
    bool _twist_draw_front_cap = true;
    bool _twist_draw_back_cap = true;

    static auto New(const cg::TriangleMesh& mesh, const std::string& meshName)
    {
        return new TwistProxy{ mesh, meshName };
    }

    const char* const meshName() const
    {
        return _meshName.c_str();
    }

    void setMesh(const cg::TriangleMesh& mesh, const std::string& meshName)
    {
        ((cg::TriangleMeshMapper*)PrimitiveProxy::mapper())->setMesh(mesh);
        _meshName = meshName;
    }

private:

    std::string _meshName;

    TwistProxy(const cg::TriangleMesh& mesh, const std::string& meshName) :
        PrimitiveProxy{ *new cg::TriangleMeshMapper{mesh} },
        _meshName{ meshName }
    {
        // do nothing
    }

};