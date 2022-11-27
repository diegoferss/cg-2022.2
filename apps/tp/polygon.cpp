#include "polygon.h"

#include <algorithm>
#include <cassert>

#include "math/Quaternion.h"

cg::Polygon::Polygon(int np)
    : points_{new vec3f[np]}, center_{}, radius_{1}, num_points_{np} {
  assert(np >= 3 && np <= 20);

  const float angle = 360.0f / np;
  const quatf rot_quat{angle, {0, 0, 1}};

  vec3f curr_point{0, 1, 0};

  for (int i = 0; i < np; i++, curr_point = rot_quat.rotate(curr_point))
    points_[i] = curr_point;
}

cg::Polygon cg::Polygon::Copy(const Polygon& src) {
  cg::Polygon ret_val{src.num_points_};
  std::copy(src.points_, src.points_ + src.num_points_, ret_val.points_);
  ret_val.center_ = src.center_;
  ret_val.radius_ = src.radius_;
  return ret_val;
}

cg::Polygon::Polygon(Polygon&& other) noexcept
    : points_{std::move(other.points_)},
      center_{std::move(other.center_)},
      radius_{std::move(other.radius_)},
      num_points_{std::move(other.num_points_)} {
  other.points_ = nullptr;
  other.center_ = {};
  other.radius_ = 0.0f;
  other.num_points_ = 0;
}

cg::Polygon& cg::Polygon::operator=(Polygon&& other) noexcept {
  points_ = std::move(other.points_);
  center_ = std::move(other.center_);
  radius_ = std::move(other.radius_);
  num_points_ = std::move(other.num_points_);
  other.points_ = nullptr;
  other.center_ = {};
  other.radius_ = 0.0f;
  other.num_points_ = 0;
  return *this;
}

cg::Polygon::~Polygon() { delete[] points_; }

cg::Polygon& cg::Polygon::Translate(const vec3f& xyz) {
  for (int i{}; i < num_points_; i++) points_[i] += xyz;
  center_ += xyz;
  return *this;
}

cg::Polygon& cg::Polygon::Rotate(float deg, const vec3f& axis) {
  auto rot_quat = quatf{deg, axis};
  for (int i{}; i < num_points_; i++) points_[i] = rot_quat.rotate(points_[i]);
  center_ = rot_quat.rotate(center_);
  return *this;
}

cg::Polygon& cg::Polygon::RotateInPlace(float deg, const vec3f& axis) {
  for (int i{}; i < num_points_; i++)
    points_[i] = quatf{deg, axis}.rotate(points_[i] - center_) + center_;
  return *this;
}

cg::Polygon& cg::Polygon::Scale(float scale) {
  for (int i{}; i < num_points_; i++)
    points_[i] = (points_[i] - center_) * scale + center_;
  radius_ *= scale;
  return *this;
}

cg::Polygon& cg::Polygon::AbsoluteScale(float scale) {
  for (int i{}; i < num_points_; i++)
    points_[i] = (points_[i] - center_).versor() * scale + center_;
  radius_ = scale;
  return *this;
}

void cg::Polygon::Draw(GLGraphics3* g3, bool draw_center) const {
  if (draw_center) g3->drawPoint(center_);
  for (int i = 0; i < num_points_ - 1; i++)
    g3->drawLine(points_[i], points_[i + 1]);
  g3->drawLine(points_[num_points_ - 1], points_[0]);
}

const cg::vec3f& cg::Polygon::Center() const { return center_; }

float cg::Polygon::Radius() const { return radius_; }

int cg::Polygon::Sides() const { return num_points_; }

cg::vec3f& cg::Polygon::operator[](int index) { return points_[index]; }

const cg::vec3f& cg::Polygon::operator[](int index) const {
  return points_[index];
}