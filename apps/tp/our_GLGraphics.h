#include "graphics/GLGraphics3.h"
#include "geometry/TriangleMesh.h"
#include "MainWindow.h"

class OurGLGraphics : public cg::GLGraphics3 {
public:
	static cg::TriangleMesh* spiral();
	static cg::TriangleMesh* twist();
};