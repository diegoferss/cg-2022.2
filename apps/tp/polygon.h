#ifndef DS_MYAPP_GENERATRIX_H_
#define DS_MYAPP_GENERATRIX_H_

#include "graphics/GLGraphics3.h"
#include "math/Vector3.h"

namespace cg {

class Polygon {
 public:
  Polygon(int num_subdiv = 6);

  // Sem cópias implícitas
  Polygon(const Polygon&) = delete;
  Polygon& operator=(const Polygon&) = delete;

  // Só copiar explicitamente, pra minha sanidade
  virtual Polygon Copy(const Polygon& src);

  // Mover a gnt deixa
  Polygon(Polygon&& src) noexcept;
  virtual Polygon& operator=(Polygon&& src) noexcept;

  ~Polygon();

  Polygon& Translate(const vec3f& xyz);
  Polygon& Rotate(float angle, const vec3f& axis = {0, 1, 0});
  Polygon& RotateInPlace(float angle, const vec3f& axis = {0, 1, 0});
  Polygon& Scale(float factor);
  Polygon& AbsoluteScale(float factor);

  virtual void Draw(GLGraphics3* g3, bool draw_center = false) const;

  const vec3f& Center() const;
  float Radius() const;
  int Sides() const;

  vec3f& operator[](int);
  const vec3f& operator[](int) const;

 protected:
  vec3f* points_;
  vec3f center_;
  float radius_;
  int num_points_;
};

}  // namespace cg

#endif  // DS_MYAPP_GENERATRIX_H_