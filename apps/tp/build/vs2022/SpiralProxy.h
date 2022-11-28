class SpiralProxy final : public cg::graph::PrimitiveProxy
{
public:

    float _arc_angle = 360;
    bool _is_polygon = false;
    bool _arc_polyline_situation = false;
    int _spiral_num_subdiv = 40;
    int _generatrix_subdiv = 20;
    float _generatrix_radius = 1;
    float _spiral_initial_radius = 2;
    float _spiral_num_revolutions = 2;
    float _spiral_height_inc = 3;
    float _spiral_radius_inc = 0.01;
    float _polygon_radius = 1;
    bool _spiral_draw_front_cap = false;
    bool _spiral_draw_back_cap = false;

    static auto New(const cg::TriangleMesh& mesh, const std::string& meshName)
    {
        return new SpiralProxy{ mesh, meshName };
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

    SpiralProxy(const cg::TriangleMesh& mesh, const std::string& meshName) :
        PrimitiveProxy{ *new cg::TriangleMeshMapper{mesh} },
        _meshName{ meshName }
    {
        // do nothing
    }

};