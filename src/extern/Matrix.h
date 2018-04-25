// Commented by Yuxi Luo, April 2018

#pragma once

// class Matrix
// provides mathematics for matrix calculation
class Matrix {
public:
    // constructor
    Matrix();

    // use union to utilize memory
    union {
        float m[4][4];
        float ml[16];
    };

    // identity matrix
    void Identity();
    // inverse of matrix
    Matrix Inverse() const;
    // matrix multiplication
    Matrix operator * (const Matrix &m2) const;

    // set translation matrix
    void Translate(float x, float y, float z);
    // set scaling matrix
    void Scale(float x, float y, float z);
    // set rotation matrix
    void Rotate(float rotation);
    void Roll(float roll);
    void Pitch(float pitch);
    void Yaw(float yaw);

    // set position matrix
    void SetPosition(float x, float y, float z);
    // set scaling matrix
    void SetScale(float x, float y, float z);
    void SetRotation(float rotation);
    // set rotation matrix
    void SetRoll(float roll);
    void SetPitch(float pitch);
    void SetYaw(float yaw);

    // set orthographic projection
    void SetOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
    // set perspective projection
    void SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
};
