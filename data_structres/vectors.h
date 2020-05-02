#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

namespace flux {

// ----- 2D Vector Implementation ------
struct Vector2D {
  float x, y;
  Vector2D() : x(0), y(0) {}
  Vector2D(float x, float y) : x(x), y(y) {}
  inline Vector2D &operator+=(const Vector2D &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  inline Vector2D &operator-=(const Vector2D &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  inline Vector2D &operator*=(const float &c) {
    x *= c;
    y *= c;
    return *this;
  }
  inline Vector2D &operator/=(const float &c) {
    x /= c;
    y /= c;
    return *this;
  }
  inline float magnitude() {
    return sqrtf(x * x + y * y);
  }
  inline Vector2D rotate(float cos_theta, float sin_theta) {
    return Vector2D(x * cos_theta - y * sin_theta,
                    x * sin_theta + y * cos_theta);
  }
};

inline bool operator==(const Vector2D &v1, const Vector2D &v2) {
  return v1.x == v2.x && v1.y == v2.y;
}
inline bool operator!=(const Vector2D &v1, const Vector2D &v2) {
  return !(v1 == v2);
}
inline Vector2D operator-(const Vector2D &v) {
  return Vector2D(-v.x, -v.y);
}
inline Vector2D operator+(const Vector2D &v1, const Vector2D &v2) {
  return Vector2D(v1.x + v2.x, v1.y + v2.y);
}
inline Vector2D operator-(const Vector2D &v1, const Vector2D &v2) {
  return v1 + -v2;
}
inline Vector2D operator*(const float &c, const Vector2D &v) {
  return Vector2D(v.x * c, v.y * c);
}
inline Vector2D operator*(const Vector2D &v, const float &c) {
  return c * v;
}
inline Vector2D operator/(const Vector2D &v, const float &c) {
  return Vector2D(v.x / c, v.y / c);
}
// -------------------------------------

// ----- 3D Vector Implementation ------
struct Vector3D {
  float x, y, z;
  Vector3D() : x(0), y(0), z(0) {}
  Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
  inline Vector3D &operator+=(const Vector3D &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  inline Vector3D &operator-=(const Vector3D &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  inline Vector3D &operator*=(const float &c) {
    x *= c;
    y *= c;
    z *= c;
    return *this;
  }
  inline Vector3D &operator/=(const float &c) {
    x /= c;
    y /= c;
    z /= c;
    return *this;
  }
  inline float magnitude() {
    return sqrtf(x * x + y * y + z * z);
  }
};

inline bool operator==(const Vector3D &v1, const Vector3D &v2) {
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}
inline bool operator!=(const Vector3D &v1, const Vector3D &v2) {
  return !(v1 == v2);
}
inline Vector3D operator-(const Vector3D &v) {
  return Vector3D(-v.x, -v.y, -v.z);
}
inline Vector3D operator+(const Vector3D &v1, const Vector3D &v2) {
  return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline Vector3D operator-(const Vector3D &v1, const Vector3D &v2) {
  return v1 + -v2;
}
inline Vector3D operator*(const float &c, const Vector3D &v) {
  return Vector3D(v.x * c, v.y * c, v.z * c);
}
inline Vector3D operator*(const Vector3D &v, const float &c) {
  return c * v;
}
inline Vector3D operator/(const Vector3D &v, const float &c) {
  return Vector3D(v.x / c, v.y / c, v.z / c);
}
// -------------------------------------

// ------- Vector Math Functions -------
namespace vector {

inline float dot(const Vector2D &v1, const Vector2D &v2) {
  return v1.x * v2.x + v1.y * v2.y;
}
inline float dot(const Vector3D &v1, const Vector3D &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vector3D cross(const Vector3D &v1, const Vector3D &v2) {
  float x = v1.y * v2.z - v1.z * v2.y;
  float y = v1.z * v2.x - v1.x * v2.z;
  float z = v1.x * v2.y - v1.y * v2.x;
  return Vector3D(x, y, z);
}

}
// -------------------------------------

} // namespace flux

#endif // VECTORS_H