#ifndef DS_MYAPP_POLYGON_H_
#define DS_MYAPP_POLYGON_H_

#include "generatrix.h"

namespace cg {

	class Polygon final : public Generatrix {
	public:
		Polygon(int = 3);
	};

}  // namespace cg

#endif  // DS_MYAPP_POLYGON_H_
