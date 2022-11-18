#ifndef DS_MYAPP_MESH_SWEEPS_H_
#define DS_MYAPP_MESH_SWEEPS_H_

#include <memory>

#include "generatrix.h"
#include "geometry/TriangleMesh.h"

namespace cg {

TriangleMesh* MakeSpiral(Generatrix& generatrix, int num_subdiv,
                         float initial_radius, float num_revolutions,
                         float height_inc, float radius_inc, GLGraphics3* g3,
                         bool draw_front_cap = true, bool draw_back_cap = true,
                         bool draw_generatrices = false);

TriangleMesh* MakeTwist(Generatrix& generatrix, int num_subdiv,
                        float num_revolutions, float length_inc,
                        float vert_displ, float hor_displ, float initial_scale,
                        float final_scale, GLGraphics3* g3,
                        bool draw_front_cap = true, bool draw_back_cap = true,
                        bool draw_generatrices = false);

}  // namespace cg

#endif  // DS_MYAPP_MESH_SWEEPS_H_