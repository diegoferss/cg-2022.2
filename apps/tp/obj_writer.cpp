#include "obj_writer.h"

void WriteMesh(const cg::TriangleMesh* mesh, FILE* out) {
  if (!out) return;
  auto vertices = mesh->data().vertices;
  for (int i{}; i < mesh->data().vertexCount; i++, vertices++) {
    fprintf_s(out, "v %f %f %f\n", vertices->x, vertices->y, vertices->z);
  }
  auto normals = mesh->data().vertexNormals;
  for (int i{}; i < mesh->data().vertexCount; i++, normals++) {
    fprintf_s(out, "vn %f %f %f\n", normals->x, normals->y, normals->z);
  }
  auto triangles = mesh->data().triangles;
  for (int i{}; i < mesh->data().triangleCount; i++, triangles++) {
    fprintf_s(out, "f %d %d %d\n", triangles->v[0] + 1, triangles->v[1] + 1,
              triangles->v[2] + 1);
  }
}