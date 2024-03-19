#pragma once //インクルードガード

template <typename T>
class VectorBase {
protected:
    T x;
    T y;
    T z;

public:
    VectorBase(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

    void set(T newX, T newY, T newZ) {
        x = newX;
        y = newY;
        z = newZ;
    }

    void setX(T newX) {
        x = newX;
    }
    void setY(T newY) {
        y = newY;
    }
    void setZ(T newZ) {
        z = newZ;
    }

    VectorBase<T> copy() const {
        return VectorBase<T>(x, y, z);
    }

    VectorBase<T> diff(const VectorBase<T> &other) const {
        return VectorBase<T>(x - other.x, y - other.y, z - other.z);
    }

    VectorBase<T> scale(T scaleFactor) const {
        return VectorBase<T>(x * scaleFactor, y * scaleFactor, z * scaleFactor);
    }

    T getX() const {
        return x;
    }

    T getY() const {
        return y;
    }

    T getZ() const {
        return z;
    }
};

// template <typename T>
// class Vector3 : public VectorBase<T> {
// public:
//     using VectorBase<T>::x;
//     using VectorBase<T>::y;
//     using VectorBase<T>::z;

//     Vector3(T x = 0, T y = 0, T z = 0) : VectorBase<T>(x, y, z) {}

//     void add(const Vector3<T> &vec) {
//         this->x += vec.x;
//         this->y += vec.y;
//         this->z += vec.z;
//     }

// };
template <typename T>
class Vector3 : public VectorBase<T> {
public:
    using VectorBase<T>::x;
    using VectorBase<T>::y;
    using VectorBase<T>::z;

    Vector3(T x = 0, T y = 0, T z = 0) : VectorBase<T>(x, y, z) {}

    void add(const Vector3<T> &vec) {
        this->x += vec.x;
        this->y += vec.y;
        this->z += vec.z;
    }

     Vector3 cross(const Vector3& other) const {
        return Vector3(this->y * other.z - this->z * other.y,
                       this->z * other.x - this->x * other.z,
                       this->x * other.y - this->y * other.x);
    }

    Vector3& operator*=(T scaleFactor) {
        this->x *= scaleFactor;
        this->y *= scaleFactor;
        this->z *= scaleFactor;
        return *this;
    }
    Vector3& operator+=(const Vector3& other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    Vector3& operator/=(T divisor) {
        if (divisor != 0) {
            this->x /= divisor;
            this->y /= divisor;
            this->z /= divisor;
        }
        return *this;
    }

    

    // Vector3 normalize() const {
    //     float length = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    //     return Vector3(this->x / length, this->y / length, this->z / length);
    // }

    float length() const {
        return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    Vector3 normalize() const {
        float len = length();
        if (len != 0) {
            return Vector3(this->x / len, this->y / len, this->z / len);
        } else {
            return Vector3(0.0f, 0.0f, 0.0f); // return a zero vector if the length is zero
        }
    }

    Vector3<T> scale(T scaleFactor) const {
        return Vector3<T>(this->x * scaleFactor, this->y * scaleFactor, this->z * scaleFactor);
    }

    Vector3<T> diff(const Vector3<T> &other) const {
        return Vector3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
    }
};



template <typename T>
class Vector2 : public VectorBase<T> {
public:
    Vector2(T x = 0, T y = 0) : VectorBase<T>(x, y, 0) {}

    void set(T newX, T newY) {
        VectorBase<T>::set(newX, newY, 0);
    }

    Vector2<T> copy() const {
        return Vector2<T>(this->x, this->y);
    }
};

