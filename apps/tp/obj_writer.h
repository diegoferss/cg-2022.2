#ifndef DS_MYAPP_OBJ_WRITER_H_
#define DS_MYAPP_OBJ_WRITER_H_

#include "geometry/TriangleMesh.h"

void WriteMesh(const cg::TriangleMesh* mesh = nullptr, FILE* stream = stdout);

#endif  // DS_MYAPP_OBJ_WRITER_H_