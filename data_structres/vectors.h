#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

namespace flux {

// ----- 2D Vector Implementation ------
typedef struct vector_2d_t {
  float x, y;
  vector_2d_t(float x, float y) : x(x), y(y) {}
  inline vector_2d_t &operator+=(const vector_2d_t &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  inline vector_2d_t &operator-=(const vector_2d_t &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  inline vector_2d_t &operator*=(const float &c) {
    x *= c;
    y *= c;
    return *this;
  }
  inline vector_2d_t &operator/=(const float &c) {
    x /= c;
    y /= c;
    return *this;
  }
} Vector2D;

inline bool operator==(const vector_2d_t &v1, const vector_2d_t &v2) {
  return v1.x == v2.x && v1.y == v2.y;
}
inline bool operator!=(const vector_2d_t &v1, const vector_2d_t &v2) {
  return !(v1 == v2);
}
inline vector_2d_t operator-(const vector_2d_t &v) {
  return vector_2d_t(-v.x, -v.y);
}
inline vector_2d_t operator+(const vector_2d_t &v1, const vector_2d_t &v2) {
  return vector_2d_t(v1.x + v2.x, v1.y + v2.y);
}
inline vector_2d_t operator-(const vector_2d_t &v1, const vector_2d_t &v2) {
  return v1 + -v2;
}
inline vector_2d_t operator*(const float &c, const vector_2d_t &v) {
  return vector_2d_t(v.x * c, v.y * c);
}
inline vector_2d_t operator*(const vector_2d_t &v, const float &c) {
  return c * v;
}
inline vector_2d_t operator/(const vector_2d_t &v, const float &c) {
  return vector_2d_t(v.x / c, v.y / c);
}
// -------------------------------------

// ----- 3D Vector Implementation ------
typedef struct vector_3d_t {
  float x, y, z;
  vector_3d_t(float x, float y, float z) : x(x), y(y), z(z) {}
  inline vector_3d_t &operator+=(const vector_3d_t &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  inline vector_3d_t &operator-=(const vector_3d_t &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  inline vector_3d_t &operator*=(const float &c) {
    x *= c;
    y *= c;
    z *= c;
    return *this;
  }
  inline vector_3d_t &operator/=(const float &c) {
    x /= c;
    y /= c;
    z /= c;
    return *this;
  }
} Vector3D;

inline bool operator==(const vector_3d_t &v1, const vector_3d_t &v2) {
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}
inline bool operator!=(const vector_3d_t &v1, const vector_3d_t &v2) {
  return !(v1 == v2);
}
inline vector_3d_t operator-(const vector_3d_t &v) {
  return vector_3d_t(-v.x, -v.y, -v.z);
}
inline vector_3d_t operator+(const vector_3d_t &v1, const vector_3d_t &v2) {
  return vector_3d_t(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline vector_3d_t operator-(const vector_3d_t &v1, const vector_3d_t &v2) {
  return v1 + -v2;
}
inline vector_3d_t operator*(const float &c, const vector_3d_t &v) {
  return vector_3d_t(v.x * c, v.y * c, v.z * c);
}
inline vector_3d_t operator*(const vector_3d_t &v, const float &c) {
  return c * v;
}
inline vector_3d_t operator/(const vector_3d_t &v, const float &c) {
  return vector_3d_t(v.x / c, v.y / c, v.z / c);
}
// -------------------------------------

// ------- Vector Math Functions -------
inline float magnitude(const vector_2d_t &v) {
  return sqrtf(v.x * v.x + v.y * v.y);
}
inline float magnitude(const vector_3d_t &v) {
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
inline float dot(const vector_2d_t &v1, const vector_2d_t &v2) {
  return v1.x * v2.x + v1.y * v2.y;
}
inline float dot(const vector_3d_t &v1, const vector_3d_t &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline vector_3d_t cross(const vector_3d_t &v1, const vector_3d_t &v2) {
  float x = v1.y * v2.z - v1.z * v2.y;
  float y = v1.z * v2.x - v1.x * v2.z;
  float z = v1.x * v2.y - v1.y * v2.x;
  return vector_3d_t(x, y, z);
}
// -------------------------------------

} // namespace flux

#endif // VECTORS_H