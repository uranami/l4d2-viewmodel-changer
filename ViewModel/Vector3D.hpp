#pragma once

struct Vector3D
{
    float x, y, z;

    Vector3D() : x(0), y(0), z(0) {}


    Vector3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }


    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D operator*(float scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }


    float Dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float LengthSqr() const {
        return x * x + y * y + z * z;
    }

};
