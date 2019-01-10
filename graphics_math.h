#pragma once

#include <math.h>

union Vector3 {
    float v[3];
    struct {
        float x;
        float y;
        float z;
    };
};

union Vector4 {
    float v[4];
    struct {
        float x;
        float y;
        float z;
        float w;
    };
};

union Quaternion {
    float q[4];
    struct {
        float x;
        float y;
        float z;
        float w;
    };
};

union Matrix4 {
    float m[4][4];
};

Vector3 vector3(float x, float y, float z){
    Vector3 v;
    v.v[0] = x;
    v.v[1] = y;
    v.v[2] = z;
    return v;
}

Vector3 operator*(Vector3 v, f32 f){
    return vector3(v.x * f, v.y * f, v.z * f);
}

void operator+=(Vector3& v1, Vector3 v2){
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
}

void operator-=(Vector3& v1, Vector3 v2){
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
}

Vector4 vector4(float x, float y, float z, float w){
    Vector4 v;
    v.v[0] = x;
    v.v[1] = y;
    v.v[2] = z;
    v.v[3] = w;
    return v;
}

Quaternion quaternion(float x, float y, float z, float w){
    Quaternion q;
    q.q[0] = x;
    q.q[1] = y;
    q.q[2] = z;
    q.q[3] = w;
    return q;
}

Vector3 normalize(Vector3 v){
    float length = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    if(length == 0){
        v.x = 0, v.y = 0, v.z = 0;
    }else{
        v.x /= length;
        v.y /= length;
        v.z /= length;
    }
    return v;
}

void normalize(Vector3* v){
    float length = sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
    if(length == 0){
        v->x = 0, v->y = 0, v->z = 0;
    }else{
        v->x /= length;
        v->y /= length;
        v->z /= length;
    }
}

void normalize(Quaternion* q){
    float length = sqrt((q->x * q->x) + (q->y * q->y) + (q->z * q->z) + (q->w * q->w));
    if(length == 0){
        q->x = 0, q->y = 0, q->z = 0, q->w = 0;
    }else{
        q->x /= length;
        q->y /= length;
        q->z /= length;
        q->w /= length;
    }
}

Quaternion rotationToQuaternion(Vector3 axis, f32 angle){
    f32 hang = angle / 2.0f;
    Quaternion q;
    f32 sHang = sin(hang);
    q.x = axis.x * sHang;
    q.y = axis.y * sHang;
    q.z = axis.z * sHang;
    q.w = cos(hang);
    normalize(&q);
    return q;
}

Matrix4 quaternionToMatrix4(Quaternion q){
    normalize(&q);

    Matrix4 m;

    m.m[0][0] = 1 - (2 * (q.y * q.y)) - (2 * (q.z * q.z));
    m.m[0][1] = (2 * q.x * q.y) + (2 * q.z * q.w);
    m.m[0][2] = (2 * q.x * q.z) - (2 * q.y * q.w);
    m.m[0][3] = 0;

    m.m[1][0] = (2 * q.x * q.y) - (2 * q.z * q.w);
    m.m[1][1] = 1 - (2 * q.x * q.x) - (2 * (q.z *  q.z));
    m.m[1][2] = 2 * (q.y * q.z) + 2 * (q.x * q.w);
    m.m[1][3] = 0;

    m.m[2][0] = (2 * q.x * q.z) + (2 * q.y * q.w);
    m.m[2][1] = (2 * q.y  * q.z) - (2 * q.x * q.w);
    m.m[2][2] = 1 - (2 * q.x * q.x) - (2 * (q.y *  q.y));
    m.m[2][3] = 0;

    m.m[3][0] = 0;
    m.m[3][1] = 0;
    m.m[3][2] = 0;
    m.m[3][3] = 1;

    return m;
}

Quaternion multiply(Quaternion q1, Quaternion q2){
    Quaternion q;
    q.x =   q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    q.y =  -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    q.z =   q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    q.w =  -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
    return q;
}

void rotate(Quaternion* q, Vector3 angle, f32 amount){
    Quaternion r = rotationToQuaternion(angle, amount);
    *q = multiply(*q, r);
}

void scale(Matrix4* m, Vector3* v){
    m->m[0][0] += v->x;
    m->m[1][1] += v->y;
    m->m[2][2] += v->z;
}

void translate(Matrix4* m, Vector3* v){
    m->m[3][0] += v->x;
    m->m[3][1] += v->y;
    m->m[3][2] += v->z;
}

void setIdentityMatrix(Matrix4* m){
    m->m[0][0] = 1; m->m[0][1] = 0; m->m[0][2] = 0; m->m[0][3] = 0;
    m->m[1][0] = 0; m->m[1][1] = 1; m->m[1][2] = 0; m->m[1][3] = 0;
    m->m[2][0] = 0; m->m[2][1] = 0; m->m[2][2] = 1; m->m[2][3] = 0;
    m->m[3][0] = 0; m->m[3][1] = 0; m->m[3][2] = 0; m->m[3][3] = 1;
}

Matrix4 generateIdentityMatrix(){
    Matrix4 m = {};
    m.m[0][0] = 1;
    m.m[1][1] = 1;
    m.m[2][2] = 1;
    m.m[3][3] = 1;
    return m;
}

Matrix4 multiply(Matrix4* m1, Matrix4* m2){
    Matrix4 m = {};

    m.m[0][0] = (m1->m[0][0] * m2->m[0][0]) + (m1->m[1][0] * m2->m[0][1]) + (m1->m[2][0] * m2->m[0][2]) + (m1->m[3][0] * m2->m[0][3]);
    m.m[0][1] = (m1->m[0][1] * m2->m[0][0]) + (m1->m[1][1] * m2->m[0][1]) + (m1->m[2][1] * m2->m[0][2]) + (m1->m[3][1] * m2->m[0][3]);
    m.m[0][2] = (m1->m[0][2] * m2->m[0][0]) + (m1->m[1][2] * m2->m[0][1]) + (m1->m[2][2] * m2->m[0][2]) + (m1->m[3][2] * m2->m[0][3]);
    m.m[0][3] = (m1->m[0][3] * m2->m[0][0]) + (m1->m[1][3] * m2->m[0][1]) + (m1->m[2][3] * m2->m[0][2]) + (m1->m[3][3] * m2->m[0][3]);

    m.m[1][0] = (m1->m[0][0] * m2->m[1][0]) + (m1->m[1][0] * m2->m[1][1]) + (m1->m[2][0] * m2->m[1][2]) + (m1->m[3][0] * m2->m[1][3]);
    m.m[1][1] = (m1->m[0][1] * m2->m[1][0]) + (m1->m[1][1] * m2->m[1][1]) + (m1->m[2][1] * m2->m[1][2]) + (m1->m[3][1] * m2->m[1][3]);
    m.m[1][2] = (m1->m[0][2] * m2->m[1][0]) + (m1->m[1][2] * m2->m[1][1]) + (m1->m[2][2] * m2->m[1][2]) + (m1->m[3][2] * m2->m[1][3]);
    m.m[1][3] = (m1->m[0][3] * m2->m[1][0]) + (m1->m[1][3] * m2->m[1][1]) + (m1->m[2][3] * m2->m[1][2]) + (m1->m[3][3] * m2->m[1][3]);

    m.m[2][0] = (m1->m[0][0] * m2->m[2][0]) + (m1->m[1][0] * m2->m[2][1]) + (m1->m[2][0] * m2->m[2][2]) + (m1->m[3][0] * m2->m[2][3]);
    m.m[2][1] = (m1->m[0][1] * m2->m[2][0]) + (m1->m[1][1] * m2->m[2][1]) + (m1->m[2][1] * m2->m[2][2]) + (m1->m[3][1] * m2->m[2][3]);
    m.m[2][2] = (m1->m[0][2] * m2->m[2][0]) + (m1->m[1][2] * m2->m[2][1]) + (m1->m[2][2] * m2->m[2][2]) + (m1->m[3][2] * m2->m[2][3]);
    m.m[2][3] = (m1->m[0][3] * m2->m[2][0]) + (m1->m[1][3] * m2->m[2][1]) + (m1->m[2][3] * m2->m[2][2]) + (m1->m[3][3] * m2->m[2][3]);

    m.m[3][0] = (m1->m[0][0] * m2->m[3][0]) + (m1->m[1][0] * m2->m[3][1]) + (m1->m[2][0] * m2->m[3][2]) + (m1->m[3][0] * m2->m[3][3]);
    m.m[3][1] = (m1->m[0][1] * m2->m[3][0]) + (m1->m[1][1] * m2->m[3][1]) + (m1->m[2][1] * m2->m[3][2]) + (m1->m[3][1] * m2->m[3][3]);
    m.m[3][2] = (m1->m[0][2] * m2->m[3][0]) + (m1->m[1][2] * m2->m[3][1]) + (m1->m[2][2] * m2->m[3][2]) + (m1->m[3][2] * m2->m[3][3]);
    m.m[3][3] = (m1->m[0][3] * m2->m[3][0]) + (m1->m[1][3] * m2->m[3][1]) + (m1->m[2][3] * m2->m[3][2]) + (m1->m[3][3] * m2->m[3][3]);

    return m;
}