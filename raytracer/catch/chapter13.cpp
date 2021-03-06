#include "catch.hpp"
#include "cylinder.h"
#include "shape.h"
#include "equal.h"

TEST_CASE("A ray misses a cylinder")
{
  Cylinder cyl;

  const std::vector<std::tuple<Point, Vector>> data{
      {Point{1, 0,  0}, Vector{0, 1, 0}},
      {Point{0, 0,  0}, Vector{0, 1, 0}},
      {Point{0,  0, -5}, Vector{1, 1, 1}}
  };

  for (const auto& [origin, direction]: data) {
    const auto dir = direction.normalize();
    const auto r = Ray(origin, dir);
    const auto xs = cyl.local_intersect(r);

    REQUIRE(xs.empty());
  }
}


TEST_CASE("A ray strikes a cylinder")
{
  Cylinder cyl;

  const std::vector<std::tuple<Point, Vector, float, float>> data{
      {Point{1, 0,  -5}, Vector{0, 0, 1}, 5, 5},
      {Point{0, 0,  -5}, Vector{0, 0, 1}, 4, 6},
      {Point{0.5,  0, -5}, Vector{0.1, 1, 1}, 6.80798, 7.08872}
  };

  for (const auto& [origin, direction, t0, t1]: data) {
    const auto dir = direction.normalize();
    const auto r = Ray(origin, dir);
    const auto xs = cyl.local_intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t, t0));
    REQUIRE(equal(xs.at(1).t, t1));
  }
}

TEST_CASE("Normal vector on a cylinder")
{
  Cylinder cyl;

  const std::vector<std::tuple<Point, Vector>> data{
      {Point{1, 0,  0},  Vector{1, 0, 0}},
      {Point{0, 5,  -1}, Vector{0, 0, -1}},
      {Point{0, -2, 1},  Vector{0, 0, 1}},
      {Point{-1, 1, 0},  Vector{-1, 0, 0}}
  };

  for (const auto& [point, normal]: data) {
    const auto n = cyl.local_normal_at(point);
    REQUIRE(n == normal);
  }
}

TEST_CASE("The default minimum and maximum for a cylinder")
{
  Cylinder cyl;

  REQUIRE(cyl.minimum == -INFINITY);
  REQUIRE(cyl.maximum == INFINITY);
}

TEST_CASE("Intersecting a constrained cylinder")
{
  Cylinder cyl;
  cyl.minimum = 1;
  cyl.maximum = 2;

  const std::vector<std::tuple<Point, Vector, size_t>> data{
      {Point{0, 1.5, 0}, Vector{0.1, 1, 0}, 0},
      {Point{0, 3,  -5}, Vector{0, 0, 1}, 0},
      {Point{0, 0, -5}, Vector{0, 0, 1}, 0},
      {Point{0, 2, -5}, Vector{0, 0, 1}, 0},
      {Point{0, 1, -5}, Vector{0, 0, 1}, 0},
      {Point{0, 1.5, -2}, Vector{0, 0, 1}, 2}
  };

  for (const auto& [point, direction, count]: data) {
    const auto dir = direction.normalize();
    const auto r = Ray{point, dir};
    const auto xs = cyl.local_intersect(r);

    REQUIRE(xs.size() == count);
  }
}

TEST_CASE("The default closed value for a cylinder")
{
  Cylinder cyl;

  REQUIRE_FALSE(cyl.closed);
}

TEST_CASE("Intersecting the caps of a closed cylinder")
{
  Cylinder cyl;
  cyl.minimum = 1;
  cyl.maximum = 2;
  cyl.closed = true;

  const std::vector<std::tuple<Point, Vector, size_t>> data{
      {Point{0, 3, 0},   Vector{0, -1, 0}, 2},
      {Point{0, 3, -2},  Vector{0, -1, 2}, 2},
      {Point{0, 4, -2},  Vector{0, -1, 1}, 2},
      {Point{0, 0, -2},  Vector{0, 1, 2},  2},
      {Point{0, -1, -2}, Vector{0, 1, 1},  2}
  };

  for (const auto& [point, direction, count]: data) {
    const auto dir = direction.normalize();
    const auto r = Ray{point, dir};
    const auto xs = cyl.local_intersect(r);

    REQUIRE(xs.size() == count);
  }
}

TEST_CASE("The normal vector on a cylinder's end caps")
{
  Cylinder cyl;
  cyl.minimum = 1;
  cyl.maximum = 2;
  cyl.closed = true;

  const std::vector<std::tuple<Point, Vector>> data{
      {Point{0, 1, 0},   Vector{0, -1, 0}},
      {Point{0.5, 1, 0}, Vector{0, -1, 0}},
      {Point{0, 1, 0.5}, Vector{0, -1, 0}},
      {Point{0, 2, 0},   Vector{0, 1, 0}},
      {Point{0.5, 2, 0}, Vector{0, 1, 0}},
      {Point{0, 2, 0.5}, Vector{0, 1, 0}}
  };

  for (const auto& [point, normal]: data) {
    const auto n = cyl.local_normal_at(point);
    REQUIRE(n == normal);
  }
}
