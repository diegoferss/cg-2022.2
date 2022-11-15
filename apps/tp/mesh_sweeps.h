#ifndef DS_MYAPP_MESH_SWEEPS_H_
#define DS_MYAPP_MESH_SWEEPS_H_

#include <memory>

#include "generatrix.h"
#include "geometry/TriangleMesh.h"

namespace cg {

    /// <summary>
    /// Makes a spiral :D
    /// </summary>
    /// <param name="generatrix">Polygon or circular arc.</param>
    /// <param name="num_subdiv">Number of subdivisions.</param>
    /// <param name="initial_radius">Initial radius of the spiral.</param>
    /// <param name="num_revolutions">Number of revolutions.</param>
    /// <param name="height_inc">How much to increment height by every iteration.</param>
    /// <param name="radius_inc">How much to increment radius by every iteration.</param>
    /// <param name="to_be_removed">TEMPORARY. GLGraphics3 instance for drawing generatrices.</param>
    /// <param name="draw_front_cap">Whether or not to draw the front cap of the screw.</param>
    /// <param name="draw_back_cap">Whether or not to draw the back cap of the screw.</param>
    /// <param name="draw_generatrices">TEMPORARY. Whether or not to draw the generatrices.</param>
    /// <returns>A pointer to the newly generated mesh.</returns>
    TriangleMesh* MakeSpiral(Generatrix&, int, float, float, float, float,
        GLGraphics3*, bool = true, bool = true, bool = false);

    /// <summary>
    /// Makes a screw. Kinda.
    /// </summary>
    /// <param name="generatrix">A polygon or circular arc.</param>
    /// <param name="num_subdiv">Number of subdivisions.</param>
    /// <param name="num_revolutions">Number of revolutions.</param>
    /// <param name="length_inc">Amount to increment length by every iteration.</param>
    /// <param name="vertical_translation">Total vertical displacement.</param>
    /// <param name="horizontal_translation">Total horizontal displacement.</param>
    /// <param name="initial_scale">Radius of the base of the screw.</param>
    /// <param name="final_scale">Radius of the top of the screw.</param>
    /// <param name="to_be_removed">TEMPORARY. GLGraphics3 object for drawing generatrices.</param>
    /// <param name="draw_front_cap">Whether or not to draw the front cap of the screw.</param>
    /// <param name="draw_back_cap">Whether or not to draw the back cap of the screw.</param>
    /// <param name="draw_generatrices">TEMPORARY. Whether or not to draw the generatrices.</param>
    /// <returns>A pointer to the newly generated mesh.</returns>
    TriangleMesh* MakeTwist(Generatrix&, int, float, float, float, float, float,
        float, GLGraphics3*, bool = true, bool = true,
        bool = false);

}  // namespace cg

#endif  // DS_MYAPP_MESH_SWEEPS_H_  