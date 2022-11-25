#ifndef DS_TP_ARC_H_
#define DS_TP_ARC_H_

#include <algorithm>
#include <cassert>

#include "generatrix.h"
#include "graphics/GLGraphics3.h"
#include "math/Quaternion.h"
#include "math/Vector3.h"

namespace cg {

class Arc : public Generatrix {
 public:
  Arc(int nc = 2, float angle = 360, bool close_polyline = false)
      : radius_{1}, close_polyline_{close_polyline} {
    assert(nc >= 2 && nc <= 40);
    assert(angle > 0 && angle <= 360);

    np_ = angle == 360 ? nc : nc + 1;
    arcAngle_ = angle;

    points_ = new vec3f[np_];

    const float realAngle = angle / nc;
    const quatf rot_quat{realAngle, {0, 0, 1}};

    // making the arc symmetrical wrt the yz plane
    vec3f curr_point{quatf{-angle / 2, {0, 0, 1}}.rotate({0, 1, 0})};

    for (int i = 0; i < np_; i++, curr_point = rot_quat.rotate(curr_point))
      points_[i] = curr_point;
  }

  Arc(const Arc&) = delete;
  Arc& operator=(const Arc&) = delete;

  static Arc Copy(const Arc& src) {
    Arc ret_val{src.np_};
    std::copy(src.points_, src.points_ + src.np_, ret_val.points_);
    ret_val.center_ = src.center_;
    ret_val.radius_ = src.radius_;
    return ret_val;
  }

  Arc(Arc&& other) noexcept
      : points_{std::move(other.points_)},
        center_{std::move(other.center_)},
        radius_{std::move(other.radius_)},
        arcAngle_{std::move(other.arcAngle_)},
        close_polyline_{std::move(other.close_polyline_)},
        np_{std::move(other.np_)} {
    other.points_ = nullptr;
    other.center_ = {};
    other.arcAngle_ = 0.0f;
    other.close_polyline_ = false;
    other.np_ = 0;
  }

  Arc& operator=(Arc&& other) noexcept {
    points_ = std::move(other.points_);
    center_ = std::move(other.center_);
    radius_ = std::move(other.radius_);
    np_ = std::move(other.np_);
    other.points_ = nullptr;
    other.center_ = {};
    other.radius_ = other.np_ = 0;
  }

  ~Arc() { delete[] points_; }

  Arc& Translate(const vec3f& xyz) {
    for (int i{}; i < np_; i++) points_[i] += xyz;
    center_ += xyz;
    return *this;
  }

  Arc& Rotate(float deg, const vec3f& axis = {0, 0, 1}) {
    auto rot_quat = quatf{deg, axis};
    for (int i{}; i < np_; i++) points_[i] = rot_quat.rotate(points_[i]);
    center_ = rot_quat.rotate(center_);
    return *this;
  }

  Arc& RotateInPlace(float deg, const cg::vec3f& axis = {0, 0, 1}) {
    for (int i{}; i < np_; i++)
      points_[i] = quatf{deg, axis}.rotate(points_[i] - center_) + center_;
    return *this;
  }

  Arc& Scale(float scale) {
    for (int i{}; i < np_; i++)
      points_[i] = (points_[i] - center_) * scale + center_;
    radius_ *= scale;
    return *this;
  }

  void Draw(cg::GLGraphics3* g3, bool draw_center = false) const {
    for (int i = 0; i < np_ - 1; i++) g3->drawLine(points_[i], points_[i + 1]);

    if (close_polyline_ || arcAngle_ == 360)
      g3->drawLine(points_[np_ - 1], points_[0]);
  }

  int Sides() const { return np_; }

  const vec3f& Center() const { return center_; }

  float Radius() const { return radius_; }

  vec3f& operator[](int index) { return points_[index]; }

  const vec3f& operator[](int index) const { return points_[index]; }

 private:
  vec3f* points_;
  vec3f center_;
  float radius_;
  float arcAngle_;
  bool close_polyline_;
  int np_;
};  //

}  // namespace cg

#endif