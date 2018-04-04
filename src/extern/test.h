
#pragma once

class Matrix {
public:
    Matrix();

    union {
        float m[4][4];
        float ml[16];
    };

    void Identity();
    Matrix operator * (const Matrix &m2) const;
    Matrix Inverse() const;

    void Translate(float x, float y, float z);
    void Scale(float x, float y, float z);
    void Rotate(float rotation);
    void Roll(float roll);
    void Pitch(float pitch);
    void Yaw(float yaw);

    void SetPosition(float x, float y, float z);
    void SetScale(float x, float y, float z);
    void SetRotation(float rotation);
    void SetRoll(float roll);
    void SetPitch(float pitch);
    void SetYaw(float yaw);

    void SetOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
    void SetPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
};
