#ifndef __Transform_h
#define __Transform_h

#include "graph/Component.h"
#include "math/Matrix4x4.h"

namespace cg
{ 

    namespace graph
    { 
        class SpiralParameters final : public Component
        {
        public:
            SpiralParameters();

            SpiralParameters* parent() const; // implemented in SceneObject.h

             int numSubdiv() const
            {
                return _num_subdiv;
            }

             float initialRadius() const
            {
                return _initial_radius;
            }

             float numRevolutions() const
            {
                return _num_revolutions;
            }

             float heightInc() const
            {
                return _height_inc;
            }

             float radiusInc() const
            {
                return _radius_inc;
            }

             bool drawFrontCap() const
            {
                return _draw_front_cap;
            }

             bool drawBackCap() const
            {
                return _draw_back_cap;
            }

             bool drawGeneratrices() const
            {
                return draw_generatrices;
            }


        private:
            int _num_subdiv;
            float _initial_radius;
            float _num_revolutions;
            float _height_inc;
            float _radius_inc;
            bool _draw_front_cap;
            bool _draw_back_cap;
            bool draw_generatrices;

            void update() override;

            friend class SceneObject;

        }; // Transform

    } // end namespace graph

} // end namespace cg

#endif // __Transform_h
