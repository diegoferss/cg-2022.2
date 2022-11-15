#include "generatrix.h"

#include <cassert>
#include <algorithm>

#include "math/Quaternion.h"

cg::Generatrix::Generatrix(int np)
    : points_{ new vec3f[np] }, center_{}, radius_{ 1 }, num_points_{ np } {
    assert(np >= 3 && np <= 20);

    const float angle = 360.0f / np;
    const quatf rot_quat{ angle, {0, 0, 1} };

    vec3f curr_point{ 0, 1, 0 };

    for (int i = 0; i < np; i++, curr_point = rot_quat.rotate(curr_point))
        points_[i] = curr_point;
}


cg::Generatrix cg::Generatrix::Copy(const Generatrix& src) {
    cg::Generatrix ret_val{ src.num_points_ };
    std::copy(src.points_, src.points_ + src.num_points_, ret_val.points_);
    ret_val.center_ = src.center_;
    ret_val.radius_ = src.radius_;
    return ret_val;
}

cg::Generatrix::Generatrix(Generatrix&& other) noexcept
    : points_{ std::move(other.points_) },
    center_{ std::move(other.center_) },
    radius_{ std::move(other.radius_) },
    num_points_{ std::move(other.num_points_) } {
    other.points_ = nullptr;
    other.center_ = {};
    other.radius_ = 0.0f;
    other.num_points_ = 0;
}

cg::Generatrix& cg::Generatrix::operator=(Generatrix&& other) noexcept {
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

cg::Generatrix::~Generatrix() { delete[] points_; }

cg::Generatrix& cg::Generatrix::Translate(const vec3f& xyz) {
    for (int i{}; i < num_points_; i++) points_[i] += xyz;
    center_ += xyz;
    return *this;
}

cg::Generatrix& cg::Generatrix::Rotate(float deg, const vec3f& axis) {
    auto rot_quat = quatf{ deg, axis };
    for (int i{}; i < num_points_; i++) points_[i] = rot_quat.rotate(points_[i]);
    center_ = rot_quat.rotate(center_);
    return *this;
}

cg::Generatrix& cg::Generatrix::RotateInPlace(float deg,
    const vec3f& axis) {
    for (int i{}; i < num_points_; i++)
        points_[i] = quatf{ deg, axis }.rotate(points_[i] - center_) + center_;
    return *this;
}

cg::Generatrix& cg::Generatrix::Scale(float scale) {
    for (int i{}; i < num_points_; i++)
        points_[i] = (points_[i] - center_) * scale + center_;
    radius_ *= scale;
    return *this;
}

cg::Generatrix& cg::Generatrix::TwistScale(float scale) {
    for (int i{}; i < num_points_; i++)  // TODO: may be (num_points_ - 1)
        points_[i] = (points_[i] - center_).versor() * scale + center_;
    radius_ = scale;
    return *this;
}

void cg::Generatrix::Draw(GLGraphics3* g3, bool draw_center) const {
    if (draw_center) g3->drawPoint(center_);
    for (int i = 0; i < num_points_ - 1; i++)
        g3->drawLine(points_[i], points_[i + 1]);
    g3->drawLine(points_[num_points_ - 1], points_[0]);
}

const cg::vec3f& cg::Generatrix::Center() const { return center_; }

float cg::Generatrix::Radius() const { return radius_; }

int cg::Generatrix::Sides() const { return num_points_; }

cg::vec3f& cg::Generatrix::operator[](int index) { return points_[index]; }

const cg::vec3f& cg::Generatrix::operator[](int index) const {
    return points_[index];
}