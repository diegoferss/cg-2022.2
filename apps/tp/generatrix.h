#ifndef DS_MYAPP_GENERATRIX_H_
#define DS_MYAPP_GENERATRIX_H_

#include "graphics/GLGraphics3.h"
#include "math/Vector3.h"

namespace cg {

class Generatrix {
 public:
  Generatrix(int num_subdiv = 3);

  // Sem cópias implícitas
  Generatrix(const Generatrix&) = delete;
  Generatrix& operator=(const Generatrix&) = delete;

  // Só copiar explicitamente, pra minha sanidade
  virtual Generatrix Copy(const Generatrix& src);

  // Mover a gnt deixa
  Generatrix(Generatrix&& src) noexcept;
  virtual Generatrix& operator=(Generatrix&& src) noexcept;

  ~Generatrix();

  Generatrix& Translate(const vec3f& xyz);
  Generatrix& Rotate(float angle, const vec3f& axis = {0, 1, 0});
  Generatrix& RotateInPlace(float angle, const vec3f& axis = {0, 1, 0});
  Generatrix& Scale(float factor);
  Generatrix& AbsoluteScale(float factor);

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