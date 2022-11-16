#ifndef DS_MYAPP_GENERATRIX_H_
#define DS_MYAPP_GENERATRIX_H_

#include "graphics/GLGraphics3.h"
#include "math/Vector3.h"

namespace cg {

class Generatrix {
 public:
  Generatrix(int = 3);

  // Sem cópias implícitas
  Generatrix(const Generatrix&) = delete;
  Generatrix& operator=(const Generatrix&) = delete;

  // Só copiar explicitamente, pra minha sanidade
  virtual Generatrix Copy(const Generatrix&);

  // Mover a gnt deixa
  Generatrix(Generatrix&&) noexcept;
  virtual Generatrix& operator=(Generatrix&&) noexcept;

  ~Generatrix();

  Generatrix& Translate(const vec3f&);
  Generatrix& Rotate(float, const vec3f& = {0, 1, 0});
  Generatrix& RotateInPlace(float, const vec3f& = {0, 1, 0});
  Generatrix& Scale(float);
  Generatrix& TwistScale(float);

  virtual void Draw(GLGraphics3*, bool = false) const;

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