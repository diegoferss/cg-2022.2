#ifndef DS_MYAPP_MESH_SWEEPS_H_
#define DS_MYAPP_MESH_SWEEPS_H_

#include <memory>

#include "polygon.h"
#include "arc.h"
#include "geometry/TriangleMesh.h"

namespace cg {

TriangleMesh* MakeSpiral(Polygon& generatrix, int num_subdiv,
                         float initial_radius, float num_revolutions,
                         float height_inc, float radius_inc,
                         bool draw_front_cap = true, bool draw_back_cap = true,
                         bool draw_generatrices = false);

TriangleMesh* MakeSpiral(Arc& generatrix, int num_subdiv,
                        float initial_radius, float num_revolutions,
                        float height_inc, float radius_inc,
                        bool draw_front_cap = true, bool draw_back_cap = true,
                        bool draw_generatrices = false);

TriangleMesh* MakeTwist(Polygon& generatrix, int num_subdiv,
                        float num_revolutions, float length_inc,
                        float vert_displ, float hor_displ, float initial_scale,
                        float final_scale,
                        bool draw_front_cap = true, bool draw_back_cap = true,
                        bool draw_generatrices = false);

TriangleMesh* MakeTwist(Arc& generatrix, int num_subdiv,
                        float num_revolutions, float length_inc,
                        float vert_displ, float hor_displ, float initial_scale,
                        float final_scale,
                        bool draw_front_cap = true, bool draw_back_cap = true,
                        bool draw_generatrices = false);

}  // namespace cg

#endif  // DS_MYAPP_MESH_SWEEPS_H_